//
//  blockchain.hpp
//
//

#ifndef blockchain_hpp
#define blockchain_hpp

#include "src/utils/typeDefs.hpp"

#include <vector>


namespace mining_game {

    class Block;
    struct BlockchainSettings;

    class Blockchain {
    private:
        Value valueNetworkTotal;
        BlockTime timeInSecs;
        BlockRate secondsPerBlock;
        ValueRate transactionFeeRate;

        size_t _maxHeightPub;
        std::vector<std::vector<std::unique_ptr<Block>>> blocks;
    public:
        Blockchain(BlockchainSettings blockchainSettings);

        void advanceToTime(BlockTime time);
        void addBlock(std::unique_ptr<Block> block);

        Block &winningHead() const;
        Value txPooled(BlockTime period) const;
        TimeRate chanceToWin(HashRate hashRate) const;
        const std::vector<std::unique_ptr<Block>> & frontier() const;

        BlockTime getTime() const { return timeInSecs; };
        BlockTime getMaxHeightPub() const { return _maxHeightPub; };
    };
}

#endif /* blockchain_hpp */
