//
//  main.cpp
//
//

#include "src/learning_model/exp3.hpp"
#include "src/learning_model/strategy.hpp"
#include "src/learning_model/player_profile.hpp"
#include "src/mining_game/miner.hpp"
#include "src/mining_game/miner_group.hpp"
#include "src/mining_game/game.hpp"
#include "src/mining_game/blockchain.hpp"
#include "src/strategy_behaviour/default_behaviour.hpp"
#include "src/strategy_behaviour/petty_behaviour.hpp"
#include "src/strategy_behaviour/lazy_fork_behaviour.hpp"
#include "src/strategy_behaviour/payforward_behaviour.hpp"
#include "src/strategy_behaviour/function_fork_behaviour.hpp"
#include "src/utils/utils.hpp"
#include "src/utils/parser.hpp"

#include <vector>
#include <iostream>
#include <fstream>
#include <sys/stat.h>
#include <string>
#include <map>
#include <sstream>

namespace LM = learning_model;
namespace MG = mining_game;
namespace SB = strategy_behaviour;

struct RunSettings {
    unsigned int numberOfGames;
    MinerCount totalMiners;
    MinerCount fixedDefault;
    MG::GameSettings gameSettings;
    std::string folderPrefix;
};

std::string makeResultsFolder(std::string resultFolder);
Value calculateMaxProfit(RunSettings settings);
void run(RunSettings settings);
void writeWeights(unsigned int gameNum, LM::Exp3 model, std::vector<std::ofstream> & outputStreams);


void writeWeights(unsigned int gameNum, LM::Exp3 model, std::vector<std::ofstream> & outputStreams) {
    auto strategyWeights = model.getStrategyWeights();
    for (size_t i = 0; i < strategyWeights.size(); i++) {
        outputStreams[i] << gameNum << " " << strategyWeights[i] << std::endl;
    }
}

std::string makeResultsFolder(std::string resultsFolder) {
    char final [256];
    sprintf (final, "./%s", resultsFolder.c_str());
    mkdir(final,0775);
    return resultsFolder;
}

Value calculateMaxProfit(RunSettings settings) {
    auto secsPerBlock(settings.gameSettings.blockchainSettings.secondsPerBlock);
    auto expNumBlocks(settings.gameSettings.blockchainSettings.numberOfBlocks);
    Value moneyLeftAtEnd(0);
    BlockValue transactionFeeRate(settings.gameSettings.blockchainSettings.transactionFeeRate);
    Value transactionFeesTaken(transactionFeeRate * (expNumBlocks * secsPerBlock) - moneyLeftAtEnd);

    return (double) transactionFeesTaken / Value(rawCount(settings.totalMiners) / 4);
}

void run(RunSettings settings) {

    std::vector<std::unique_ptr<LM::Strategy>> learningStrategies;
    StratWeight defaultWeight(1);

    auto honest = std::make_unique<SB::DefaultBehaviour>();
    auto petty = std::make_unique<SB::PettyBehaviour>();
    auto payforward = std::make_unique<SB::PayforwardBehaviour>();
    auto lazyFork = std::make_unique<SB::LazyForkBehaviour>();
    std::vector<std::unique_ptr<SB::Behaviour>> funcForks;

    std::map<std::string, SB::Behaviour *> strategies {
        {"honest", honest.get()},
        {"petty", petty.get()},
        {"payforward", payforward.get()},
        {"lazy", lazyFork.get()},
    };

    auto resultFolder = makeResultsFolder(settings.folderPrefix);
    auto defaultStrategy(std::make_unique<LM::Strategy>("honest", defaultWeight, honest.get()));

    std::vector<std::ofstream> outputStreams;
    double defaultWeightScale(1);
    double defaultLambertCoefficient(0.13533528323661);
    //coeff for lambert func equil  must be in [0,.2]
    //0.13533528323661 = 1/(e^2)
    Value expectedTxFees(
        settings.gameSettings.blockchainSettings.secondsPerBlock * settings.gameSettings.blockchainSettings.transactionFeeRate);
    
    for (auto &strategy : settings.gameSettings.strategies) {
        std::vector<std::string> s = utils::split(strategy, ':');
        std::string name = s[0];
        double weightScaling = s.size() == 2 ? stod(s[1]) : defaultWeightScale;
        if (name.substr(0, 4) == "fork") {
            std::vector<std::string> f = utils::split(name, '-');
            assert(f.size() == 2);
            int coeff = stoi(f[1]);
            funcForks.push_back(std::make_unique<SB::FunctionForkBehaviour>(
                SB::FunctionForkBehaviour::forkWithCoefficient(coeff)
            ));
            strategies[name] = funcForks.back().get();
        } else if (name.substr(0, 7) == "lambert") {
            std::vector<std::string> l = utils::split(name, '-');
            double coeff = l.size() == 2 ? stod(l[1]) : defaultLambertCoefficient;
            funcForks.push_back(std::make_unique<SB::FunctionForkBehaviour>(
                SB::FunctionForkBehaviour::lambertWithCoefficient(coeff, expectedTxFees)
            ));
            strategies[name] = funcForks.back().get();
        }
        learningStrategies.push_back(std::make_unique<LM::Strategy>(strategy, weightScaling * defaultWeight, strategies[name]));
        outputStreams.push_back(std::ofstream(resultFolder + "/" + strategy + ".txt"));
    }

    std::vector<LM::Strategy *> expLearningStrategies;
    for(auto &s: learningStrategies) {
        expLearningStrategies.push_back(s.get());
    }

    double phi = .01;
    LM::Exp3 model = LM::Exp3(expLearningStrategies, phi);
    MinerCount numberRandomMiners(settings.totalMiners - settings.fixedDefault);
    std::vector<LM::PlayerProfile> minerProfiles = model.pickStrategiesEvenly(numberRandomMiners);

    auto blockchain = std::make_unique<MG::Blockchain>(settings.gameSettings.blockchainSettings);
    auto minerGroup = MG::MinerGroup::build(
        settings.totalMiners,
        settings.fixedDefault,
        expLearningStrategies,
        defaultStrategy.get(),
        *blockchain.get());

    // NOTE: max profit is unpredictable in pay forward game since miners subsidise with external funds
    // NOTE: max profit calculated is in case of no pay forward
    auto maxProfit = calculateMaxProfit(settings);
    MG::Game game(settings.gameSettings);

    writeWeights(0, model, outputStreams);

    for (unsigned int gameNum = 0; gameNum < settings.numberOfGames; gameNum++) {
        blockchain->reset();
        minerGroup->reset(*blockchain.get());

        minerGroup->updateLearningMinerStrategies(minerProfiles);
        auto results = game.run(*minerGroup.get(), *blockchain.get());

        // NOTE: implicit assumption that the first n miners are learners
        // where n is the number of learning miners! Do not change without
        // considering that.

        assert(results.minerResults.size() >= minerProfiles.size());
        for (size_t i = 0; i < minerProfiles.size(); i++) {
            minerProfiles[i].currentReward = results.minerResults[i].totalProfit;
        }

        minerProfiles = model.updateStrategyProfiles(minerProfiles, maxProfit);
        writeWeights(gameNum, model, outputStreams);
    }
}


int main(int argc, char * argv[]) {
    auto parser = utils::Parser();
    auto args = parser.parse(argc, argv);

    Value satoshiPerBitcoin(100000000); // search SATOSHI_PER_BITCOIN in original project
    BlockCount expectedNumberOfBlocks(10000); // EXPECTED_NUMBER_OF_BLOCKS
    BlockRate expectedTimeToFindBlock(600); // SEC_PER_BLOCK
    BlockValue blockReward(args.blockReward * satoshiPerBitcoin); // BLOCK_REWARD
    BlockValue transactionFeeRate((args.txFees1Block * satoshiPerBitcoin)/expectedTimeToFindBlock);  //TRANSACTION_FEE_RATE
    Value payforward(args.payforward * satoshiPerBitcoin);

    MG::BlockchainSettings blockchainSettings = {
        expectedTimeToFindBlock,
        transactionFeeRate,
        blockReward,
        expectedNumberOfBlocks,
        payforward
    };

    MG::GameSettings gameSettings = {blockchainSettings, args.commentary, args.strategies};
    // RunSettings runSettings = {1000, MinerCount(200), MinerCount(0), gameSettings, "test"};
    RunSettings runSettings = {args.numGames, args.minerCount, args.defaultMinerCount, gameSettings, args.out};
    run(runSettings);
}
