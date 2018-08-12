
#include "lazy_fork_behaviour.hpp"

#include "src/utils/utils.hpp"

#include "src/mining_game/block.hpp"
#include "src/mining_game/miner.hpp"
#include "src/mining_game/blockchain.hpp"

#include <vector>



namespace strategy_behaviour {

    LazyForkBehaviour::LazyForkBehaviour() : Behaviour() {}

    MG::Block & LazyForkBehaviour::chooseParent(const MG::Blockchain & chain, const MG::Miner & miner) const {
        if (shouldUndercut(chain) && (chain.getMaxHeightPub() != 0)) {
            return chain.most(chain.frontier(-1));
        } else {
            return chain.most(chain.frontier());
        }
    }

    // MG::Block & publish(const MG::Blockchain & chain, const & miner) const {
    //
    // }

    Value LazyForkBehaviour::collectFees(const MG::Blockchain & chain, const MG::Miner & miner, const MG::Block & parent, Value txFeesAvailable) const {
        return txFeesAvailable / Value(2);
    }

    Value LazyForkBehaviour::payForward(const MG::Blockchain & chain, const MG::Miner & miner, const MG::Block & parent, Value fees) const {
        return Value(0);
    }

    bool LazyForkBehaviour::shouldUndercut(const MG::Blockchain & chain) const {
        return chain.gap() > chain.mostRem(chain.frontier());
    }

}
