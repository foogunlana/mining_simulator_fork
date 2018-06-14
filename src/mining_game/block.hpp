
#ifndef block_hpp
#define block_hpp

#include "src/utils/typeDefs.hpp"

#include <vector>

namespace mining_game {

    class Miner;

    struct BlockParameters {
        BlockTime minedAt;
        BlockTime publishedAt;
        Value txFees;
        Value fixedReward;
        Value rem;
        Value payForward;
    };

    class Block {
    private:
        Value gain;
        Value loss;
    public:
        Miner * miner;
        Block * parent;
        BlockParameters params;
        const size_t height;

        Value valueInChain;

        Block(BlockParameters params);
        Block(Block * parent, Miner * miner, BlockParameters params);

        void reset(Block *parent_, Miner * miner_, BlockParameters params_);
        Profit realValue() const;
        std::vector<const Block *> getChain() const;
    };
}

#endif
