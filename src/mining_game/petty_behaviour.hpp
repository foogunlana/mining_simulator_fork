
#ifndef petty_behaviour_hpp
#define petty_behaviour_hpp

#include "src/utils/typeDefs.hpp"
#include "src/learning_model/behaviour.hpp"

#include <string>


namespace mining_game {

    class Blockchain;
    class Block;
    class Miner;

    class PettyBehaviour : public learning_model::Behaviour {
    public:
        PettyBehaviour();

        Block & chooseParent(const Blockchain & chain, const Miner & miner) const override;
        // Block & publish(const Blockchain & chain, const & miner) const;
        Value collectFees(const Blockchain & chain, const Miner & miner, const Block & block, Value txFeesAvailable) const override;
        Value payForward(const Blockchain & chain, const Miner & miner, const Block & block, Value fees) const override;
    };
}

#endif
