
#ifndef function_fork_behaviour_hpp
#define function_fork_behaviour_hpp

#include "behaviour.hpp"

#include <string>
#include <functional>

namespace strategy_behaviour {

    class FunctionForkBehaviour : public Behaviour {
    private:
        std::function<Value(Value)> f;
    public:
        FunctionForkBehaviour(std::function<Value(Value)> f);

        MG::Block & chooseParent(const MG::Blockchain & chain, const MG::Miner & miner) const override;
        // MG::Block & publish(const MG::Blockchain & chain, const & miner) const;
        Value collectFees(const MG::Blockchain & chain, const MG::Miner & miner, const MG::Block & block, Value txFeesAvailable) const override;
        Value payForward(const MG::Blockchain & chain, const MG::Miner & miner, const MG::Block & block, Value fees) const override;

        bool shouldUndercut(const MG::Blockchain & chain) const;
        Value valUnder(const MG::Blockchain & chain) const;
        Value valCont(const MG::Blockchain & chain) const;
        
        static std::function<Value(Value)> forkWithCoefficient(int coefficient);
        static std::function<Value(Value)> lambertWithCoefficient(double coefficient, Value expectedTxFees);
    };
}

#endif
