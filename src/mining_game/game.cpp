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

namespace mining_game {

    Game::Game(GameSettings settings_): blockchainSettings(settings_.blockchainSettings) {}

    GameResult Game::run(MinerGroup &minerGroup, Blockchain &blockchain) {

        BlockTime endTime = blockchainSettings.numberOfBlocks * blockchainSettings.secondsPerBlock;
        BlockTime currentTime;

        auto params = BlockParameters{
            BlockTime(0),
            BlockTime(0),
            Value(0),
            Value(0),
            Value(0),
            Value(0),
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
        // int pettyCount = 0;
        // int lazyCount = 0;
        Profit totalValue(0);
        // std::vector<std::string> winners;
        // winners.resize(winningBlock.height);

        for (auto mined : winningChain) {
            if (mined->height == BlockHeight(0)) {
                break;
            }
            // winners[mined->height] = mined->miner->getStrategyName();
            if (mined->parent->miner == mined->miner) {
                parentCount++;
            }
            auto &miner = *(mined->miner);
            minerResults[miner.params.number].addBlock(mined);
            totalValue += mined->realValue();

            // if (miner.getStrategyName() == "petty") {
            //     pettyCount++;
            // } else if (miner.getStrategyName() == "lazy-fork") {
            //     lazyCount++;
            // }
        }

        // for (size_t h = 1; h < blockchain.getMaxHeightPub(); h++) {
        //     int pettyBlockCount = 0;
        //     int lazyBlockCount = 0;
        //     const std::vector<std::unique_ptr<Block>> & blocks = blockchain.frontier(h);
        //     for (const std::unique_ptr<Block> &block : blocks) {
        //         if (block->miner->getStrategyName() == "petty") {
        //             pettyBlockCount++;
        //         } else if (block->miner->getStrategyName() == "lazy-fork") {
        //             lazyBlockCount++;
        //         }
        //         std::cout << *block << std::endl;
        //     }
        //     std::cout << "winner=" << winners[h] << std::endl;
        // }
        // std::cout << "lazyFork=" << lazyCount << " & petty=" << pettyCount << std::endl;

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

        Value moneyLeftAtEnd = winningBlock.params.rem + winningBlock.params.payForward;

        GameResult result(minerResults, totalBlocks, finalBlocks, moneyLeftAtEnd, totalValue);

        // assert(winningBlock.valueInChain == totalValue);
        for (size_t i = 0; i < minerGroup.miners.size(); i++) {
            assert(minerResults[i].totalProfit <= totalValue);
        }

        // std::cout << "Total blocks mined:" << totalBlocks << " with " << finalBlocks << " making it into the final chain" << std::endl;
        return result;
    }
}
