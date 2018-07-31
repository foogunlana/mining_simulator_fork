
#include "function_fork_behaviour.hpp"

#include "src/utils/typeDefs.hpp"
#include "src/utils/utils.hpp"

#include "src/mining_game/block.hpp"
#include "src/mining_game/miner.hpp"
#include "src/mining_game/blockchain.hpp"

#include <vector>
#include <iostream>
#include <gsl/gsl_sf_lambert.h>
#include <cmath>

#define UNDERCUT_VALUE Value(100000)

namespace strategy_behaviour {

    bool shouldUndercut(const MG::Blockchain & chain);

    FunctionForkBehaviour::FunctionForkBehaviour(std::function<Value(Value)> _f) : Behaviour(), f(_f) {}

    MG::Block & FunctionForkBehaviour::chooseParent(const MG::Blockchain & chain, const MG::Miner & miner) const {
        if (shouldUndercut(chain) && (chain.getMaxHeightPub() != 0)) {
            return chain.most(chain.frontier(-1));
        } else {
            return chain.most(chain.frontier());
        }
    }

    // MG::Block & publish(const MG::Blockchain & chain, const & miner) const {
    //
    // }

    Value FunctionForkBehaviour::collectFees(const MG::Blockchain & chain, const MG::Miner & miner, const MG::Block & parent, Value txFeesAvailable) const {
        if (shouldUndercut(chain) && (chain.getMaxHeightPub() != 0)) {
            return std::min(chain.gap() - UNDERCUT_VALUE, f(txFeesAvailable));
        } else {
            return f(txFeesAvailable);
        }
    }

    Value FunctionForkBehaviour::payForward(const MG::Blockchain & chain, const MG::Miner & miner, const MG::Block & parent, Value fees) const {
        return Value(0);
    }

    bool FunctionForkBehaviour::shouldUndercut(const MG::Blockchain & chain) const {
        return valUnder(chain) > valCont(chain);
    }

    Value FunctionForkBehaviour::valUnder(const MG::Blockchain & chain) const {
        return std::min(chain.gap() - UNDERCUT_VALUE, f(chain.mostRem(chain.frontier(-1))));
    }

    Value FunctionForkBehaviour::valCont(const MG::Blockchain & chain) const {
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
