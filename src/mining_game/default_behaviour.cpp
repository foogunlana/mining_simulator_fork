
#include "default_behaviour.hpp"
#include "src/learning_model/strategy.hpp"

#include "block.hpp"
#include "miner.hpp"
#include "blockchain.hpp"

#include <vector>



namespace mining_game {

    DefaultBehaviour::DefaultBehaviour() : learning_model::Behaviour() {}

    Block & DefaultBehaviour::chooseParent(const Blockchain & chain, const Miner & miner) const {
        return *chain.frontier().front().get();
    }

    // Block & publish(const Blockchain & chain, const & miner) const {
    //
    // }

    Value DefaultBehaviour::collectFees(const Blockchain & chain, const Block & block, const Miner & miner) const {
        return Value(0);
    }

    Value DefaultBehaviour::payForward(const Blockchain & chain, const Block & block, Value fees, const Miner & miner) const {
        return Value(0);
    }
}
