
#include "default_behaviour.hpp"

// #include "src/utils/typeDefs.hpp"
#include "src/utils/utils.hpp"

#include "src/mining_game/block.hpp"
#include "src/mining_game/miner.hpp"
#include "src/mining_game/blockchain.hpp"

#include <vector>



namespace strategy_behaviour {

    DefaultBehaviour::DefaultBehaviour() : Behaviour() {}

    MG::Block & DefaultBehaviour::chooseParent(const MG::Blockchain & chain, const MG::Miner & miner) const {
        const std::vector<std::unique_ptr<MG::Block>> & possiblities = chain.frontier();
        return *possiblities[utils::selectRandomIndex(possiblities.size())].get();
    }

    // MG::Block & publish(const MG::Blockchain & chain, const & miner) const {
    //
    // }

    Value DefaultBehaviour::collectFees(const MG::Blockchain & chain, const MG::Miner & miner, const MG::Block & parent, Value txFeesAvailable) const {
        return txFeesAvailable;
    }

    Value DefaultBehaviour::payForward(const MG::Blockchain & chain, const MG::Miner & miner, const MG::Block & parent, Value fees) const {
        return Value(0);
    }
}
