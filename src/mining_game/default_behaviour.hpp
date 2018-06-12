
#ifndef default_behaviour_hpp
#define default_behaviour_hpp

#include "src/utils/typeDefs.hpp"
#include "src/learning_model/behaviour.hpp"

#include <string>


namespace mining_game {

    class Blockchain;
    class Block;
    class Miner;

    class DefaultBehaviour : public learning_model::Behaviour {
    public:
        DefaultBehaviour();

        Block & chooseParent(const Blockchain & chain, const Miner & miner) const override;
        // Block & publish(const Blockchain & chain, const & miner) const;
        Value collectFees(const Blockchain & chain, const Block & block, const Miner & miner) const override;
        Value payForward(const Blockchain & chain, const Block & block, Value fees, const Miner & miner) const override;
    };
}

#endif
