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
#include "src/mining_game/default_behaviour.hpp"
#include "src/mining_game/petty_behaviour.hpp"
#include "src/mining_game/lazy_fork_behaviour.hpp"

#include <vector>
#include <iostream>
#include <fstream>
#include <sys/stat.h>

#define LAMBERT_COEFF 0.13533528323661
//coeff for lambert func equil  must be in [0,.2]
//0.13533528323661 = 1/(e^2)

namespace LM = learning_model;
namespace MG = mining_game;

struct RunSettings {
    unsigned int numberOfGames;
    MinerCount totalMiners;
    MinerCount fixedDefault;
    MG::GameSettings gameSettings;
    std::string folderPrefix;
};

std::string makeResultsFolder(RunSettings settings);
Value calculateMaxProfit(RunSettings settings);
void run(RunSettings settings);
void writeWeights(unsigned int gameNum, LM::Exp3 model, std::vector<std::ofstream> & outputStreams);


void writeWeights(unsigned int gameNum, LM::Exp3 model, std::vector<std::ofstream> & outputStreams) {
    auto strategyWeights = model.getStrategyWeights();
    for (size_t i = 0; i < strategyWeights.size(); i++) {
        outputStreams[i] << gameNum << " " << strategyWeights[i] << '\n';
    }
}

std::string makeResultsFolder(RunSettings settings) {
    std::string resultFolder = "";
    if (settings.folderPrefix.length() > 0) {
        resultFolder += settings.folderPrefix + "-";
    }
    resultFolder += std::to_string(rawCount(settings.fixedDefault));
    char final [256];
    sprintf (final, "./%s", resultFolder.c_str());
    mkdir(final,0775);
    return resultFolder;
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

    auto honest = std::make_unique<MG::DefaultBehaviour>();
    auto petty = std::make_unique<MG::PettyBehaviour>();
    auto lazyFork = std::make_unique<MG::LazyForkBehaviour>();
    auto resultFolder = makeResultsFolder(settings);

    auto defaultStrategy(std::make_unique<LM::Strategy>("default", defaultWeight, honest.get()));

    // NOTE: better to use map here but requires getStrategies from model instead of only weights
    std::vector<std::ofstream> outputStreams;
    learningStrategies.push_back(std::make_unique<LM::Strategy>("petty", defaultWeight, petty.get()));
    outputStreams.push_back(std::ofstream(resultFolder + "/petty.txt"));

    learningStrategies.push_back(std::make_unique<LM::Strategy>("lazy-fork", defaultWeight, lazyFork.get()));
    outputStreams.push_back(std::ofstream(resultFolder + "/lazy-fork.txt"));

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

        assert(results.minerResults.size() == minerProfiles.size());
        for (size_t i = 0; i < results.minerResults.size(); i++) {
            minerProfiles[i].currentReward = results.minerResults[i].totalProfit;
        }
        minerProfiles = model.updateStrategyProfiles(minerProfiles, maxProfit);
        writeWeights(gameNum, model, outputStreams);
    }
}


int main(int, const char * []) {
    Value satoshiPerBitcoin(100000000); // search SATOSHI_PER_BITCOIN in original project
    BlockCount expectedNumberOfBlocks(10000); // EXPECTED_NUMBER_OF_BLOCKS
    BlockRate expectedTimeToFindBlock(600); // SEC_PER_BLOCK
    BlockValue blockReward(0 * satoshiPerBitcoin); // BLOCK_REWARD
    BlockValue transactionFeeRate((50 * satoshiPerBitcoin)/expectedTimeToFindBlock);  //TRANSACTION_FEE_RATE

    MG::BlockchainSettings blockchainSettings = {
        expectedTimeToFindBlock,
        transactionFeeRate,
        blockReward,
        expectedNumberOfBlocks
    };
    MG::GameSettings gameSettings = {blockchainSettings};

    // RunSettings runSettings = {1000, MinerCount(200), MinerCount(0), gameSettings, "test"};
    RunSettings runSettings = {10000, MinerCount(200), MinerCount(0), gameSettings, "results"};
    run(runSettings);

}
