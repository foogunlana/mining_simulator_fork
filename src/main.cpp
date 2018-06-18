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
void analyse(const std::vector<MG::Miner *> & miners, std::vector<LM::PlayerProfile> profiles);

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
    auto defaultStrategy(std::make_unique<LM::Strategy>("default", defaultWeight, honest.get()));

    std::vector<std::string> strategyNames{"petty", "honest"};
    learningStrategies.push_back(std::make_unique<LM::Strategy>(strategyNames[0], defaultWeight, petty.get()));
    learningStrategies.push_back(std::make_unique<LM::Strategy>(strategyNames[1], defaultWeight, honest.get()));

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

    // for (auto profile : minerProfiles) {
    //     std::cout << expLearningStrategies[profile.currentStrategy]->name << std::endl;
    // }

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

    auto strategyWeights = model.getStrategyWeights();

    for (unsigned int gameNum = 0; gameNum < settings.numberOfGames; gameNum++) {

        // NOTE: for printing
        // std::vector<StratWeight> strategyWeights = model.getStrategyWeights();

        blockchain->reset();
        minerGroup->reset(*blockchain.get());

        minerGroup->updateLearningMinerStrategies(minerProfiles);

        auto results = game.run(*minerGroup.get(), *blockchain.get());

        assert(results.minerResults.size() == minerProfiles.size());
        for (size_t i = 0; i < results.minerResults.size(); i++) {
            minerProfiles[i].currentReward = results.minerResults[i].totalProfit;
        }

        strategyWeights = model.getStrategyWeights();
        minerProfiles = model.updateStrategyProfiles(minerProfiles, maxProfit);
        analyse(minerGroup->getLearningMiners(), minerProfiles);

    }
    // model->writeWeights(settings.numberOfGames);
}

void analyse(const std::vector<MG::Miner *> & miners, std::vector<LM::PlayerProfile> profiles) {
    for (size_t miner = 0; miner < miners.size(); miner++) {
        std::cout <<
        "strategy = " << miners[miner]->getStrategyName() << " || " <<
        "reward = " << profiles[miner].currentReward << std::endl;
    }
    // for (size_t strategy = 0; strategy < weights.size(); strategy++) {
    //     std::cout << names[strategy] << "->" << weights[strategy] << "  ||  ";
    // }
    // std::cout << std::endl;
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
    RunSettings runSettings = {1, MinerCount(200), MinerCount(0), gameSettings, "test"};
    run(runSettings);

}
