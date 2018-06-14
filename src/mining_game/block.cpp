
#include "block.hpp"


namespace mining_game {

    Block::Block(BlockParameters params_) :
        miner(nullptr), parent(nullptr), params(params_), height(size_t(0))
    {
        valueInChain = Value(0);
        gain = Value(0);
        loss = Value(0);
    }

    Block::Block(const Block *parent_, const Miner * miner_, const BlockParameters params_) :
        miner(miner_), parent(parent_), params(params_), height(parent_->height + size_t(1))
    {
        valueInChain = parent->valueInChain + params.txFees + params.fixedReward;
        gain = params.txFees + params.fixedReward;
        loss = params.payForward;
    }

    Profit Block::realValue() const {
        return (Profit) gain - (Profit) loss;
    }

    std::vector<const Block *> Block::getChain() const {
        std::vector<const Block *> chain;
        const Block *current = this;
        while (current) {
            chain.push_back(current);
            current = current->parent;
        }
        return chain;
    }
}
