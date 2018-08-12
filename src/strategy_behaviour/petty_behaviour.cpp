
#include "petty_behaviour.hpp"

#include "src/utils/utils.hpp"

#include "src/mining_game/block.hpp"
#include "src/mining_game/miner.hpp"
#include "src/mining_game/blockchain.hpp"

#include <vector>



namespace strategy_behaviour {

    PettyBehaviour::PettyBehaviour() : Behaviour() {}

    MG::Block & PettyBehaviour::chooseParent(const MG::Blockchain & chain, const MG::Miner & miner) const {
        return chain.most(chain.frontier());
    }

    // MG::Block & publish(const MG::Blockchain & chain, const & miner) const {
    //
    // }

    Value PettyBehaviour::collectFees(const MG::Blockchain & chain, const MG::Miner & miner, const MG::Block & parent, Value txFeesAvailable) const {
        return txFeesAvailable;
    }

    Value PettyBehaviour::payForward(const MG::Blockchain & chain, const MG::Miner & miner, const MG::Block & parent, Value fees) const {
        return Value(0);
    }
}
