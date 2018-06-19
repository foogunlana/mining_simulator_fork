
#include "lazy_fork_behaviour.hpp"
#include "src/learning_model/behaviour.hpp"

// #include "src/utils/typeDefs.hpp"
#include "src/utils/utils.hpp"

#include "block.hpp"
#include "miner.hpp"
#include "blockchain.hpp"

#include <vector>



namespace mining_game {

    bool lazyUndercutIsProfitable(const Blockchain & chain);

    LazyForkBehaviour::LazyForkBehaviour() : learning_model::Behaviour() {}

    Block & LazyForkBehaviour::chooseParent(const Blockchain & chain, const Miner & miner) const {
        if (lazyUndercutIsProfitable(chain) && (chain.getMaxHeightPub() != 0)) {
            return chain.most(chain.frontier(-1));
        } else {
            return chain.most(chain.frontier());
        }
    }

    // Block & publish(const Blockchain & chain, const & miner) const {
    //
    // }

    Value LazyForkBehaviour::collectFees(const Blockchain & chain, const Miner & miner, const Block & block, Value txFeesAvailable) const {
        return txFeesAvailable / 2;
    }

    Value LazyForkBehaviour::payForward(const Blockchain & chain, const Miner & miner, const Block & block, Value fees) const {
        return Value(0);
    }

    // NOTE: calculation of rem is wrong here. Needs to be rem up until current time
    // This doesn't work because each block is mined at a different time... Though it might work now since there is no network delay
    bool lazyUndercutIsProfitable(const Blockchain & chain) {
        return chain.gap() >= chain.most(chain.frontier()).params.rem;
    }

}
