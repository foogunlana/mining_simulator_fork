
#include "payforward_behaviour.hpp"

// #include "src/utils/typeDefs.hpp"
#include "src/utils/utils.hpp"

#include "src/mining_game/block.hpp"
#include "src/mining_game/miner.hpp"
#include "src/mining_game/blockchain.hpp"

#include <vector>
#include <iostream>


namespace strategy_behaviour {

    MG::Block & firstPayforwardValidBlock(const MG::Blockchain & blockchain);

    PayforwardBehaviour::PayforwardBehaviour() : Behaviour() {}

    MG::Block & PayforwardBehaviour::chooseParent(const MG::Blockchain & chain, const MG::Miner & miner) const {
        if(chain.maxPayforwardValidHeight == 0) {
            const std::vector<std::unique_ptr<MG::Block>> & validFrontier = chain.frontier();
            return *validFrontier[0].get();
        }
        const std::vector<std::unique_ptr<MG::Block>> & validFrontier = chain.frontier(chain.maxPayforwardValidHeight);
        std::vector<MG::Block *> possiblities;
        for (auto &block : validFrontier) {
            if (chain.payforwardValid(*block.get())) {
                possiblities.push_back(block.get());
            }
        }
        return *possiblities[utils::selectRandomIndex(possiblities.size())];
    }

    // MG::Block & publish(const MG::Blockchain & chain, const & miner) const {
    //
    // }

    Value PayforwardBehaviour::collectFees(const MG::Blockchain & chain, const MG::Miner & miner, const MG::Block & parent, Value txFeesAvailable) const {
        return txFeesAvailable;
    }

    Value PayforwardBehaviour::payForward(const MG::Blockchain & chain, const MG::Miner & miner, const MG::Block & parent, Value fees) const {
        Value expectedTxFees = chain.expectedTxFees();
        if (fees > expectedTxFees) {
            return fees - expectedTxFees;
        } else {
            return Value(0);
        }
    }
}
