
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
        return chain.most(chain.frontier());
    }

    // Block & publish(const Blockchain & chain, const & miner) const {
    //
    // }

    Value PettyBehaviour::collectFees(const Blockchain & chain, const Miner & miner, const Block & parent, Value txFeesAvailable) const {
        return txFeesAvailable;
    }

    Value PettyBehaviour::payForward(const Blockchain & chain, const Miner & miner, const Block & parent, Value fees) const {
        return Value(0);
    }
}
