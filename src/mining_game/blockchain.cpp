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
        _maxHeightPub(0),
        maxPayforwardValidHeight(0),
        payforward(blockchainSettings.payforward)
    {
        blocks.resize(rawCount(blockchainSettings.numberOfBlocks) * 2);
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
        maxPayforwardValidHeight = BlockHeight(0);
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
        if (payforwardValid(*block.get())) {
            maxPayforwardValidHeight = block->height > maxPayforwardValidHeight ?
                block->height : maxPayforwardValidHeight;
        }
        blocks[height].push_back(std::move(block));
    }

    bool Blockchain::payforwardValid(const Block & block) const {
        return (block.realValue() - (Profit) block.params.fixedReward) == expectedTxFees();
    }

    Value Blockchain::expectedTxFees() const {
        return secondsPerBlock * transactionFeeRate;
    }

    Value Blockchain::txPooled(BlockTime period) const {
        return period * transactionFeeRate;
    }

    const std::vector<std::unique_ptr<Block>> & Blockchain::frontier() const {
        return blocks[_maxHeightPub];
    }

    const std::vector<std::unique_ptr<Block>> & Blockchain::frontier(signed int offset) const {
        if (_maxHeightPub == 0) {
            return blocks[0];
        }
        assert((_maxHeightPub - offset) <= (2 * _maxHeightPub)); //acceptable range
        auto height = offset < 0 ? _maxHeightPub + offset : offset;
        return blocks[height];
    }

    TimeRate Blockchain::chanceToWin(HashRate hashRate) const {
        return hashRate / secondsPerBlock;
    }

    Value Blockchain::rem(const Block & block) const {
        return rem(block, getTime());
    }

    Value Blockchain::rem(const Block & block, BlockTime at) const {
        auto expectedRem = block.params.rem + txPooled(at - block.params.minedAt);
        // NOTE: the assert below doesn't work with non-zero payforward since it
        // can be included in the txFeesInChain if the last miner takes PF as txFees
        // assert(expectedRem == valueNetworkTotal - block.txFeesInChain);
        return block.params.payforward + expectedRem;
    }

    Value Blockchain::gap() const {
        return mostRem(frontier(-1)) - mostRem(frontier());
    }

    Block & Blockchain::most(const std::vector<std::unique_ptr<Block>> & blocks) const {
        Value maxRem = mostRem(blocks);
        std::vector<Block *> possiblities;
        for (const auto &block : blocks) {
            if (rem(*block.get()) == maxRem) {
                possiblities.push_back(block.get());
            }
        }
        size_t index = utils::selectRandomIndex(possiblities.size());
        Block *block = possiblities[index];
        return *block;
    }

    Value Blockchain::mostRem(const std::vector<std::unique_ptr<Block>> & blocks) const {
        Value maxRem(0);
        for (const auto &block : blocks) {
            auto r = rem(*block.get());
            maxRem = r > maxRem ? r : maxRem;
        }
        return maxRem;
    }
}
