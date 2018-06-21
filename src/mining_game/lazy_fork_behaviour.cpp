
#include "lazy_fork_behaviour.hpp"
#include "src/learning_model/behaviour.hpp"

// #include "src/utils/typeDefs.hpp"
#include "src/utils/utils.hpp"

#include "block.hpp"
#include "miner.hpp"
#include "blockchain.hpp"

#include <vector>



namespace mining_game {

    bool shouldUndercut(const Blockchain & chain);

    LazyForkBehaviour::LazyForkBehaviour() : learning_model::Behaviour() {}

    Block & LazyForkBehaviour::chooseParent(const Blockchain & chain, const Miner & miner) const {
        if (shouldUndercut(chain) && (chain.getMaxHeightPub() != 0)) {
            return chain.most(chain.frontier(-1));
        } else {
            return chain.most(chain.frontier());
        }
    }

    // Block & publish(const Blockchain & chain, const & miner) const {
    //
    // }

    Value LazyForkBehaviour::collectFees(const Blockchain & chain, const Miner & miner, const Block & parent, Value txFeesAvailable) const {
        return txFeesAvailable / Value(2);
    }

    Value LazyForkBehaviour::payForward(const Blockchain & chain, const Miner & miner, const Block & parent, Value fees) const {
        return Value(0);
    }

    bool shouldUndercut(const Blockchain & chain) {
        return chain.gap() >= chain.mostRem(chain.frontier());
    }

}
