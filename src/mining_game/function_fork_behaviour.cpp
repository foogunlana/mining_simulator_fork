
#include "function_fork_behaviour.hpp"
#include "src/learning_model/behaviour.hpp"

// #include "src/utils/typeDefs.hpp"
#include "src/utils/utils.hpp"

#include "block.hpp"
#include "miner.hpp"
#include "blockchain.hpp"

#include <vector>
#include <iostream>


namespace mining_game {

    bool shouldUndercut(const Blockchain & chain);

    FunctionForkBehaviour::FunctionForkBehaviour(int _coefficient) : learning_model::Behaviour(), coefficient(_coefficient) {}

    Block & FunctionForkBehaviour::chooseParent(const Blockchain & chain, const Miner & miner) const {
        if (shouldUndercut(chain) && (chain.getMaxHeightPub() != 0)) {
            return chain.most(chain.frontier(-1));
        } else {
            return chain.most(chain.frontier());
        }
    }

    // Block & publish(const Blockchain & chain, const & miner) const {
    //
    // }

    Value FunctionForkBehaviour::collectFees(const Blockchain & chain, const Miner & miner, const Block & parent, Value txFeesAvailable) const {
        if (shouldUndercut(chain) && (chain.getMaxHeightPub() != 0)) {
            return std::min(chain.gap(), f(txFeesAvailable));
        } else {
            return f(txFeesAvailable);
        }
    }

    Value FunctionForkBehaviour::payForward(const Blockchain & chain, const Miner & miner, const Block & parent, Value fees) const {
        return Value(0);
    }

    bool FunctionForkBehaviour::shouldUndercut(const Blockchain & chain) const {
        return (valUnder(chain) >= valCont(chain));
    }

    bool FunctionForkBehaviour::valUnder(const Blockchain & chain) const {
        return std::min(chain.gap(), f(chain.mostRem(chain.frontier(-1))));
    }

    bool FunctionForkBehaviour::valCont(const Blockchain & chain) const {
        return f(chain.mostRem(chain.frontier()));
    }

    Value FunctionForkBehaviour::f(Value txFees) const {
        return txFees / Value(coefficient);
    }
}
