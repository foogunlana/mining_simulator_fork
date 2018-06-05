//
//  blockchain.cpp
//
//

#include "blockchain.hpp"
#include "blockchain_settings.hpp"


namespace mining_game {

    Blockchain::Blockchain(BlockchainSettings blockchainSettings) :
        valueNetworkTotal(0),
        timeInSecs(0),
        secondsPerBlock(blockchainSettings.secondsPerBlock),
        transactionFeeRate(blockchainSettings.transactionFeeRate),
        _maxHeightPub(0)
    {
        // _blocks.reserve(rawCount(blockchainSettings.numberOfBlocks) * 2);
        // _blocksIndex.resize(rawCount(blockchainSettings.numberOfBlocks) * 2);
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
}
