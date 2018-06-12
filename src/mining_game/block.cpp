
#include "block.hpp"



namespace mining_game {

    Block::Block(const Block *parent_, const Miner * miner_, const BlockTime minedAt_, const Value txFees_, const Value surplus_) :
        _height(parent_->_height + size_t(1)), miner(miner_), parent(parent_), minedAt(minedAt_), txFees(txFees_), surplus(surplus_)
    {
        valueInChain = parent_->valueInChain + txFees;
    }

    Block::Block() : _height(BlockHeight(0)), miner(nullptr), parent(nullptr), minedAt(BlockTime(0)), txFees(Value(0)), surplus(Value(0)), valueInChain(Value(0)) {}

}
