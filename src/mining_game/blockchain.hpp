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
    class Miner;
    struct BlockchainSettings;
    struct BlockParameters;

    class Blockchain {
    private:
        Value valueNetworkTotal;
        BlockTime timeInSecs;
        BlockRate secondsPerBlock;
        ValueRate transactionFeeRate;

        size_t _maxHeightPub;
        std::vector<std::vector<std::unique_ptr<Block>>> blocks;
        std::vector<std::unique_ptr<Block>> oldBlocks;
    public:
        Blockchain(BlockchainSettings blockchainSettings);

        void reset();
        void advanceToTime(BlockTime time);
        void addBlock(std::unique_ptr<Block> block);
        std::unique_ptr<Block> createBlock(Block *parent, Miner *miner, BlockParameters params);

        Block &winningHead() const;
        Value txPooled(BlockTime period) const;
        TimeRate chanceToWin(HashRate hashRate) const;
        const std::vector<std::unique_ptr<Block>> & frontier() const;
        const std::vector<std::unique_ptr<Block>> & frontier(signed int offset) const;

        BlockTime getTime() const { return timeInSecs; };
        BlockTime getMaxHeightPub() const { return _maxHeightPub; };

        static Block & most(const std::vector<std::unique_ptr<Block>> & blocks);
    };
}

#endif /* blockchain_hpp */
