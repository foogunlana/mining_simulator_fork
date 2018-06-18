
#include "petty_behaviour.hpp"
#include "src/learning_model/behaviour.hpp"

// #include "src/utils/typeDefs.hpp"
#include "src/utils/utils.hpp"

#include "block.hpp"
#include "miner.hpp"
#include "blockchain.hpp"

#include <vector>



namespace mining_game {

    PettyBehaviour::PettyBehaviour() : learning_model::Behaviour() {}

    Block & PettyBehaviour::chooseParent(const Blockchain & chain, const Miner & miner) const {
        const std::vector<std::unique_ptr<Block>> & possiblities = chain.frontier();
        return *possiblities[utils::selectRandomIndex(possiblities.size())].get();
    }

    // Block & publish(const Blockchain & chain, const & miner) const {
    //
    // }

    Value PettyBehaviour::collectFees(const Blockchain & chain, const Block & block, const Miner & miner) const {
        return Value(0);
    }

    Value PettyBehaviour::payForward(const Blockchain & chain, const Block & block, Value fees, const Miner & miner) const {
        return Value(0);
    }
}
