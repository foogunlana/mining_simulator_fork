
#include "block.hpp"



namespace mining_game {

    Block::Block(Block *parent_) : _height(parent_->_height + size_t(1)) {}
    Block::Block() : _height(BlockHeight(0)) {}

    void Block::hello() {

    }
}
