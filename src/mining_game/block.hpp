
#ifndef block_hpp
#define block_hpp

#include "src/utils/typeDefs.hpp"

#include <vector>

namespace mining_game {

    class Block {
    private:
        size_t _height;
    public:
        Block(Block * parent_);
        Block();
        void hello();
        BlockHeight getHeight() { return _height; }
    };
}

#endif
