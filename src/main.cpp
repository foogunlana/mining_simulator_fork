//
//  main.cpp
//
//

#include "src/learning_model/exp3.hpp"
#include "src/learning_model/strategy.hpp"
#include "src/mining_game/miner.hpp"
#include "src/mining_game/miner_group.hpp"
#include "src/mining_game/game.hpp"

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

    std::unique_ptr<LM::Strategy> defaultStrategy(std::make_unique<LM::Strategy>("default", defaultWeight));

    std::vector<std::string> strategyNames{"petty", "lazy-fork"};
    learningStrategies.push_back(std::make_unique<LM::Strategy>(strategyNames[0], defaultWeight));
    learningStrategies.push_back(std::make_unique<LM::Strategy>(strategyNames[1], defaultWeight));

    //start running games
    // BlockCount totalBlocksMined(0);
    // BlockCount blocksInLongestChain(0);
    HashRate hashRate(1.0/rawCount(settings.totalMiners));

    std::vector<std::unique_ptr<MG::Miner>> miners;
    MG::MinerParameters parameters {rawCount(1), "test", hashRate};
    miners.push_back(std::make_unique<MG::Miner>(parameters, "strategy1"));

    // std::vector<MG::Miner *> learningMiners;
    MinerCount numberRandomMiners(settings.totalMiners - settings.fixedDefault);


    for (MinerCount i = 0; i < settings.totalMiners; i++) {
        auto minerName = std::to_string(rawCount(i));
        // MinerParameters parameters {rawCount(i), minerName, hashRate};
        // miners.push_back(std::make_unique<MG::Miner>(parameters, *defaultStrategy));
        // if (i < numberRandomMiners) {
        //     learningMiners.push_back(miners.back().get());
        // }
    }

    double phi = .01;
    LM::Exp3 model = LM::Exp3(learningStrategies, phi);
    std::vector<LM::PlayerProfile> minerProfiles = model.pickStrategiesEvenly(numberRandomMiners);
    std::vector<StratWeight> strategyWeights = model.getStrategyWeights();

    // MinerGroup minerGroup(std::move(miners));
    // auto blockchain = std::make_unique<Blockchain>(settings.gameSettings.blockchainSettings);

    MG::MinerGroup minerGroup(std::move(miners));
    MG::Game game(settings.gameSettings);

    for (unsigned int gameNum = 0; gameNum < settings.numberOfGames; gameNum++) {
        // something like
        // for (size_t i = 0; i < miners.size(); i++) {
        //     miners[i].setStrategy(learningStrategies[minerProfiles.currentStrategy])
        // }

        auto results = game.run(minerGroup);

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

    RunSettings runSettings = {1, MinerCount(200), MinerCount(0), gameSettings, "test"};
    run(runSettings);

}
