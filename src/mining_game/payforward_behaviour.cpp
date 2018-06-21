
#include "payforward_behaviour.hpp"
#include "src/learning_model/behaviour.hpp"

// #include "src/utils/typeDefs.hpp"
#include "src/utils/utils.hpp"

#include "block.hpp"
#include "miner.hpp"
#include "blockchain.hpp"

#include <vector>
#include <iostream>


namespace mining_game {

    Block & firstPayforwardValidBlock(const Blockchain & blockchain);

    PayforwardBehaviour::PayforwardBehaviour() : learning_model::Behaviour() {}

    Block & PayforwardBehaviour::chooseParent(const Blockchain & chain, const Miner & miner) const {
        const std::vector<std::unique_ptr<Block>> & possiblities = chain.frontier(chain.maxPayforwardValidHeight);
        for (auto &block : possiblities) {
            if (block->params.payforward >= chain.payforward) {
                return *block.get();
            }
        }
        return *chain.frontier(0)[0].get();
    }

    // Block & publish(const Blockchain & chain, const & miner) const {
    //
    // }

    Value PayforwardBehaviour::collectFees(const Blockchain & chain, const Miner & miner, const Block & parent, Value txFeesAvailable) const {
        return txFeesAvailable;
    }

    Value PayforwardBehaviour::payForward(const Blockchain & chain, const Miner & miner, const Block & parent, Value fees) const {
        return parent.params.payforward;
    }
}
