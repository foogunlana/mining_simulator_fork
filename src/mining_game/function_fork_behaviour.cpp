
#include "function_fork_behaviour.hpp"
#include "src/learning_model/behaviour.hpp"

// #include "src/utils/typeDefs.hpp"
#include "src/utils/utils.hpp"

#include "block.hpp"
#include "miner.hpp"
#include "blockchain.hpp"

#include <vector>
#include <iostream>
#include <gsl/gsl_sf_lambert.h>
#include <cmath>

#define UNDERCUT_VALUE Value(100000)

namespace mining_game {

    bool shouldUndercut(const Blockchain & chain);

    FunctionForkBehaviour::FunctionForkBehaviour(std::function<Value(Value)> _f) : learning_model::Behaviour(), f(_f) {}

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
        return valUnder(chain) > valCont(chain);
    }

    Value FunctionForkBehaviour::valUnder(const Blockchain & chain) const {
        return std::min(chain.gap(), f(chain.mostRem(chain.frontier(-1))));
    }

    Value FunctionForkBehaviour::valCont(const Blockchain & chain) const {
        return f(chain.mostRem(chain.frontier()));
    }

    std::function<Value(Value)> FunctionForkBehaviour::forkWithCoefficient(int coeff) {
        return [coeff] (Value txFees) -> Value {
            return txFees / Value(coeff);
        };
    }

    std::function<Value(Value)> FunctionForkBehaviour::lambertWithCoefficient(double lambertCoeff, Value expectedTxFees) {
        return [lambertCoeff, expectedTxFees] (Value txFeesAvailable) -> Value {
            double expectedSizeRaw = rawValue(expectedTxFees);
            auto blockRatio = valuePercentage(txFeesAvailable, Value(expectedTxFees));
            if (blockRatio <= lambertCoeff) {
                return txFeesAvailable;
            } else if (blockRatio < 2*lambertCoeff - std::log(lambertCoeff) - 1) {

                double argToLambertFunct0 = -lambertCoeff*std::exp(blockRatio-2*lambertCoeff);
                double lambertRes = gsl_sf_lambert_W0(argToLambertFunct0);

                double newValue = -expectedSizeRaw * lambertRes;
                return Value(static_cast<ValueType>(newValue));
            } else {
                return Value(expectedTxFees);
            }
        };
    }
}
