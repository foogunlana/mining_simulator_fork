//
//  blockchain.cpp
//
//

#include "blockchain.hpp"
#include "blockchain_settings.hpp"
#include "block.hpp"

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

    void Blockchain::advanceToTime(BlockTime time) {
        valueNetworkTotal += transactionFeeRate * (time - timeInSecs);
        timeInSecs = time;
    }

    BlockTime Blockchain::getTime() const {
        return timeInSecs;
    }

    void Blockchain::addBlock(std::unique_ptr<Block> block) {
        size_t height = block->getHeight();
        _maxHeightPub = height > _maxHeightPub ? height : _maxHeightPub;
        blocks[height].push_back(std::move(block));
    }

    const std::vector<std::unique_ptr<Block>> & Blockchain::frontier() {
        return blocks[_maxHeightPub];
    }

    TimeRate Blockchain::chanceToWin(HashRate hashRate) const {
        return hashRate / secondsPerBlock;
    }
}
