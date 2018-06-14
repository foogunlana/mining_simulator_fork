
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
        const Miner * miner;
        const Block * parent;
        const BlockParameters params;
        const size_t height;

        Value valueInChain;

        Block(const BlockParameters params);
        Block(const Block * parent, const Miner * miner, const BlockParameters params);
        Profit realValue() const;
        std::vector<const Block *> getChain() const;
    };
}

#endif
