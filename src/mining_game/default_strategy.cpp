
#include "default_strategy.hpp"

#include "src/learning_model/strategy.hpp"
#include "block.hpp"
#include "miner.hpp"
#include "blockchain.hpp"

#include <vector>

namespace LM = learning_model;

namespace mining_game {

    DefaultStrategy::DefaultStrategy(std::string name, StratWeight weight) : LM::Strategy(name, weight) {}

    Block & DefaultStrategy::chooseParent(const Blockchain & chain, const Miner & miner) const {
        return *chain.frontier().front().get();
    }

    // Block & publish(const Blockchain & chain, const & miner) const {
    //
    // }

    Value DefaultStrategy::collectFees(const Blockchain & chain, const Block & block, const Miner & miner) const {
        return Value(0);
    }

    Value DefaultStrategy::payForward(const Blockchain & chain, const Block & block, Value fees, const Miner & miner) const {
        return Value(0);
    }
}
