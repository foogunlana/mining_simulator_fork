
#ifndef default_strategy_hpp
#define default_strategy_hpp

#include "src/utils/typeDefs.hpp"
#include "src/learning_model/strategy.hpp"

#include <string>

namespace mining_game {

    class Blockchain;
    class Block;
    class Miner;

    class DefaultStrategy : learning_model::Strategy {
    public:
        DefaultStrategy(std::string name, StratWeight weight);

        Block & chooseParent(const Blockchain & chain, const Miner & miner) const;
        // Block & publish(const Blockchain & chain, const & miner) const;
        Value collectFees(const Blockchain & chain, const Block & block, const Miner & miner) const;
        Value payForward(const Blockchain & chain, const Block & block, Value fees, const Miner & miner) const;
    };
}

#endif
