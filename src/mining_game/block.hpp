
#ifndef block_hpp
#define block_hpp

#include "src/utils/typeDefs.hpp"

#include <vector>

namespace mining_game {

    class Miner;

    class Block {
    private:
        size_t _height;
    public:
        const Miner * miner;
        const Block * parent;
        const BlockTime minedAt;
        const Value txFees;
        const Value surplus;

        Value valueInChain;

        Block(const Block * parent, const Miner * miner, const BlockTime minedAt, const Value txFees, const Value surplus);
        Block();

        BlockHeight getHeight() { return _height; }
    };
}

#endif
