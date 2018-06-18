
#include "lazy_fork_behaviour.hpp"
#include "src/learning_model/behaviour.hpp"

// #include "src/utils/typeDefs.hpp"
#include "src/utils/utils.hpp"

#include "block.hpp"
#include "miner.hpp"
#include "blockchain.hpp"

#include <vector>



namespace mining_game {

    Block & mostRem(const std::vector<std::unique_ptr<Block>> & blocks);

    LazyForkBehaviour::LazyForkBehaviour() : learning_model::Behaviour() {}

    Block & LazyForkBehaviour::chooseParent(const Blockchain & chain, const Miner & miner) const {
        return mostRem(chain.frontier());
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

}
