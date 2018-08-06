//
//  game.cpp
//
//

#include "miner_group.hpp"
#include "game.hpp"
#include "game_result.hpp"
#include "blockchain.hpp"
#include "blockchain_settings.hpp"
#include "block.hpp"
#include "miner_result.hpp"
#include "miner.hpp"


#include <iostream>
#include <map>

namespace mining_game {

    Game::Game(GameSettings settings_):
        blockchainSettings(settings_.blockchainSettings), strategies(settings_.strategies), commentaryOn(settings_.commentary) {}

    GameResult Game::run(MinerGroup &minerGroup, Blockchain &blockchain) {

        BlockTime endTime = blockchainSettings.numberOfBlocks * blockchainSettings.secondsPerBlock;
        BlockTime currentTime;

        auto params = BlockParameters{
            BlockTime(0),
            BlockTime(0),
            Value(0),
            blockchainSettings.blockReward,
            Value(0),
            blockchainSettings.payforward,
        };
        auto genesis = std::make_unique<Block>(params);
        blockchain.addBlock(std::move(genesis));


        do {
            currentTime = minerGroup.nextTimeBlockFound();
            blockchain.advanceToTime(currentTime);
            minerGroup.mine(blockchain, currentTime);

            // std::cout << currentTime << '\n';
        } while (currentTime < endTime);

        std::vector<MinerResult> minerResults;
        minerResults.resize(minerGroup.miners.size());

        auto &winningBlock = blockchain.winningHead();
        auto winningChain = winningBlock.getChain();

        int parentCount = 0;
        Profit totalValue(0);
        std::vector<std::string> winners;
        winners.resize(winningBlock.height+1);

        for (auto mined : winningChain) {
            if (mined->height == BlockHeight(0)) {
                break;
            }
            winners[mined->height] = mined->miner->getStrategyName();
            if (mined->parent->miner == mined->miner) {
                parentCount++;
            }
            auto &miner = *(mined->miner);
            minerResults[miner.params.number].addBlock(mined);
            totalValue += mined->realValue();
        }

        if (commentaryOn) {
            std::map<std::string, int> winningCount;
            for (auto & s: strategies) {
                winningCount[s.first] = 0;
            }
            for (auto mined : winningChain) {
                if(mined->height == 0) continue;
                winningCount[mined->miner->getStrategyName()]++;
            }
            for (size_t h = 1; h < blockchain.getMaxHeightPub(); h++) {
                const std::vector<std::unique_ptr<Block>> & blocks = blockchain.frontier(h);
                for (const std::unique_ptr<Block> &block : blocks) {
                    std::cout << *block << std::endl;
                }
                std::cout << "winner=" << winners[h] << std::endl;
            }
            std::cout << "Blocks in winning chain: " << std::endl;
            for (auto &s : strategies) {
                std::cout << " - " << s.first << ":" << s.second << " - " << winningCount[s.first] << std::endl;
            }
            std::cout << "Value in winning chain=" << winningBlock.valueInChain << std::endl;
            std::cout << "Expected value=" << endTime * blockchainSettings.transactionFeeRate << std::endl;
        }

       // std::cout << parentCount << " block mined over parent" << std::endl;
        //calculate the score at the end
        BlockCount totalBlocks(0);
        BlockCount finalBlocks(0);

        for (size_t i = 0; i < minerGroup.miners.size(); i++) {
            const auto &miner = minerGroup.miners[i];
            // std::cout << *miner.get() << " earned:" << minerResults[i].totalProfit << " mined " << miner->getBlocksMinedTotal() <<" total, of which " << minerResults[i].blocksInWinningChain << " made it into the final chain" << std::endl;
            totalBlocks += miner->getBlocksMinedTotal();
            finalBlocks += minerResults[i].blocksInWinningChain;
        }

        Value moneyLeftAtEnd = winningBlock.params.rem + winningBlock.params.payforward;

        GameResult result(minerResults, totalBlocks, finalBlocks, moneyLeftAtEnd, totalValue);

        // assert(winningBlock.valueInChain == totalValue);
        for (size_t i = 0; i < minerGroup.miners.size(); i++) {
            assert(minerResults[i].totalProfit <= totalValue);
        }

        // std::cout << "Total blocks mined:" << totalBlocks << " with " << finalBlocks << " making it into the final chain" << std::endl;
        return result;
    }
}
