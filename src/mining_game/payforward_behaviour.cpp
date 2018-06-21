
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
        return firstPayforwardValidBlock(chain);
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

    Block & firstPayforwardValidBlock(const Blockchain & blockchain) {
        bool invalidSoFar = true;
        size_t height = blockchain.getMaxHeightPub();
        std::vector<Block *> possiblities;
        while (invalidSoFar) {
            if (height == 0) {
                return *blockchain.frontier(0)[0].get();
            }
            for (auto &block : blockchain.frontier(height)) {
                if (block->params.payforward == blockchain.payforward) {
                    // valid
                    possiblities.push_back(block.get());
                    invalidSoFar = false;
                } else {
                    std::cout << "invalid!" << std::endl;
                }
            }
            height--;
        }
        return *possiblities[utils::selectRandomIndex(possiblities.size())];
    }
}
