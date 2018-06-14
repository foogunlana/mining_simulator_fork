//
//  blockchain.cpp
//
//

#include "blockchain.hpp"
#include "blockchain_settings.hpp"
#include "block.hpp"
#include "miner.hpp"
#include "src/utils/utils.hpp"

#include <iostream>

namespace mining_game {

    Blockchain::Blockchain(BlockchainSettings blockchainSettings) :
        valueNetworkTotal(0),
        timeInSecs(0),
        secondsPerBlock(blockchainSettings.secondsPerBlock),
        transactionFeeRate(blockchainSettings.transactionFeeRate),
        _maxHeightPub(0)
    {
        // _blocks.reserve(rawCount(blockchainSettings.numberOfBlocks) * 2);
        blocks.resize(rawCount(blockchainSettings.numberOfBlocks) * 2);
        // _smallestBlocks.resize(rawCount(blockchainSettings.numberOfBlocks) * 2);
        // reset(blockchainSettings);
    }

    std::unique_ptr<Block> Blockchain::createBlock(Block *parent, Miner *miner, BlockParameters params) {
        if (oldBlocks.size() == 0) {
            return std::make_unique<Block>(parent, miner, params);
        }
        auto block = std::move(oldBlocks.back());
        oldBlocks.pop_back();
        block->reset(parent, miner, params);
        return block;
    }

    void Blockchain::reset() {
        valueNetworkTotal = Value(0);
        timeInSecs = BlockTime(0);
        _maxHeightPub = BlockHeight(0);
        oldBlocks.reserve(oldBlocks.size() + blocks.size());
        for (auto &siblings : blocks) {
            for (auto &block : siblings) {
                oldBlocks.push_back(std::move(block));
            }
            siblings.clear();
        }
    }

    Block &Blockchain::winningHead() const {
        Value largestValue(0);
        for (auto &block : blocks[rawHeight(getMaxHeightPub())]) {
            largestValue = std::max(largestValue, block->valueInChain);
        }

        std::vector<Block *> possiblities;
        for (auto &block : blocks[rawHeight(getMaxHeightPub())]) {
            if (block->valueInChain == largestValue) {
                possiblities.push_back(block.get());
            }
        }
        std::uniform_int_distribution<std::size_t> vectorDis(0, possiblities.size() - 1);
        return *possiblities[utils::selectRandomIndex(possiblities.size())];
    }

    void Blockchain::advanceToTime(BlockTime time) {
        valueNetworkTotal += transactionFeeRate * (time - timeInSecs);
        timeInSecs = time;
    }

    void Blockchain::addBlock(std::unique_ptr<Block> block) {
        size_t height = block->height;
        _maxHeightPub = height > _maxHeightPub ? height : _maxHeightPub;
        blocks[height].push_back(std::move(block));
    }

    Value Blockchain::txPooled(BlockTime period) const {
        return period * transactionFeeRate;
    }

    const std::vector<std::unique_ptr<Block>> & Blockchain::frontier() const {
        return blocks[_maxHeightPub];
    }

    TimeRate Blockchain::chanceToWin(HashRate hashRate) const {
        return hashRate / secondsPerBlock;
    }
}
