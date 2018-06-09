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

#include <vector>
#include <iostream>

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

Value calculateMaxProfit(RunSettings settings);
void run(RunSettings settings);

Value calculateMaxProfit(RunSettings settings) {
    auto secsPerBlock(settings.gameSettings.blockchainSettings.secondsPerBlock);
    auto expNumBlocks(settings.gameSettings.blockchainSettings.numberOfBlocks);
    Value moneyLeftAtEnd(0);
    BlockValue transactionFeeRate(settings.gameSettings.blockchainSettings.transactionFeeRate);
    Value transactionFeesTaken(transactionFeeRate * (expNumBlocks * secsPerBlock) - moneyLeftAtEnd);

    return transactionFeesTaken / Value(rawCount(settings.totalMiners) / 4);
}

void run(RunSettings settings) {

    std::vector<std::unique_ptr<LM::Strategy>> learningStrategies;
    StratWeight defaultWeight(1);

    auto honest = std::make_unique<MG::DefaultBehaviour>().get();
    auto defaultStrategy(std::make_unique<LM::Strategy>("default", defaultWeight, honest));

    std::vector<std::string> strategyNames{"petty", "lazy-fork"};
    learningStrategies.push_back(std::make_unique<LM::Strategy>(strategyNames[0], defaultWeight, honest));
    learningStrategies.push_back(std::make_unique<LM::Strategy>(strategyNames[1], defaultWeight, honest));

    //start running games
    // BlockCount totalBlocksMined(0);
    // BlockCount blocksInLongestChain(0);

    std::vector<LM::Strategy *> expLearningStrategies;
    for(auto &s: learningStrategies) {
        expLearningStrategies.push_back(s.get());
    }

    double phi = .01;
    LM::Exp3 model = LM::Exp3(expLearningStrategies, phi);
    MinerCount numberRandomMiners(settings.totalMiners - settings.fixedDefault);
    std::vector<LM::PlayerProfile> minerProfiles = model.pickStrategiesEvenly(numberRandomMiners);

    auto minerGroup = MG::MinerGroup::build(
        settings.totalMiners, settings.fixedDefault, expLearningStrategies);

    // std::vector<StratWeight> strategyWeights = model.getStrategyWeights();

    MG::Game game(settings.gameSettings);

    for (unsigned int gameNum = 0; gameNum < settings.numberOfGames; gameNum++) {

        minerGroup->updateLearningMinerStrategies(minerProfiles);
        auto blockchain = std::make_unique<MG::Blockchain>(settings.gameSettings.blockchainSettings);
        auto results = game.run(*minerGroup.get(), *blockchain.get());

        // something like :
        // assert(results.minerResults.size() == minerProfiles.size());
        // for (size_t i = 0; i < results.size(); i++) {
        //     minerProfiles.currentReward = results.minerResults[i].profit;
        // }
        // minerProfiles = model.updateStrategyProfiles(minerProfiles, maxProfit);

        // blockchain->reset(settings.gameSettings.blockchainSettings);
        // for (size_t strategy = 0; strategy < strategyWeights.size(); strategy++) {
        //     std::cout << strategyNames[strategy] << "->" << strategyWeights[strategy] << "  ||  ";
        // }
        // std::cout << std::endl;

        // model->writeWeights(gameNum);
        // minerGroup.reset(*blockchain);
        // for (size_t i = 0; i < learningMiners.size(); i++) {
            // something like miners[i]->changeStrategy(minerProfiles[i].currentStrategy)
        // }
        // minerGroup.resetOrder();

        // auto result = runGame(minerGroup, *blockchain, settings.gameSettings);
        // something like the below!
        // for (size_t miner = 0; miner < result.size(); miner++) {
        //     minerProfiles[miner].currentReward = result[miner].reward;
        // }

        // Value maxProfit = calculateMaxProfit(settings);

        // Steps 3, 4, 5
        // minerProfiles = model.updateStrategyProfiles(minerProfiles, maxProfit);
    }
    // model->writeWeights(settings.numberOfGames);
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

    RunSettings runSettings = {1000, MinerCount(200), MinerCount(0), gameSettings, "test"};
    run(runSettings);

}
