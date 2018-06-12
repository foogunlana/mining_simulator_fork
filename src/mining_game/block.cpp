
#include "block.hpp"



namespace mining_game {

    Block::Block(const Block *parent_, const Miner * miner_) :
        _height(parent_->_height + size_t(1)), miner(miner_), parent(parent_) {}

    Block::Block() : _height(BlockHeight(0)), miner(nullptr), parent(nullptr) {}

}
