
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
        const std::vector<std::unique_ptr<Block>> & validFrontier = chain.frontier(chain.maxPayforwardValidHeight);
        std::vector<Block *> possiblities;
        for (auto &block : validFrontier) {
            if (chain.payforwardValid(*block.get())) {
                possiblities.push_back(block.get());
            }
        }
        return *possiblities[utils::selectRandomIndex(possiblities.size())];
    }

    // Block & publish(const Blockchain & chain, const & miner) const {
    //
    // }

    Value PayforwardBehaviour::collectFees(const Blockchain & chain, const Miner & miner, const Block & parent, Value txFeesAvailable) const {
        return txFeesAvailable;
    }

    Value PayforwardBehaviour::payForward(const Blockchain & chain, const Miner & miner, const Block & parent, Value fees) const {
        Value expectedTxFees = chain.expectedTxFees();
        if (fees > expectedTxFees) {
            return fees - expectedTxFees;
        } else {
            return Value(0);
        }
    }
}
