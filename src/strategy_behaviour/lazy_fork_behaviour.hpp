
#ifndef lazy_fork_behaviour_hpp
#define lazy_fork_behaviour_hpp

#include "behaviour.hpp"

#include <string>


namespace strategy_behaviour {

    class LazyForkBehaviour : public Behaviour {
    public:
        LazyForkBehaviour();

        MG::Block & chooseParent(const MG::Blockchain & chain, const MG::Miner & miner) const override;
        // MG::Block & publish(const MG::Blockchain & chain, const & miner) const;
        Value collectFees(const MG::Blockchain & chain, const MG::Miner & miner, const MG::Block & block, Value txFeesAvailable) const override;
        Value payForward(const MG::Blockchain & chain, const MG::Miner & miner, const MG::Block & block, Value fees) const override;

        bool shouldUndercut(const MG::Blockchain & chain) const;
    };
}

#endif
