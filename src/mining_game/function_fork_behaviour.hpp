
#ifndef function_fork_behaviour_hpp
#define function_fork_behaviour_hpp

#include "src/utils/typeDefs.hpp"
#include "src/learning_model/behaviour.hpp"

#include <string>
#include <functional>

namespace mining_game {

    class Blockchain;
    class Block;
    class Miner;

    class FunctionForkBehaviour : public learning_model::Behaviour {
    private:
        std::function<Value(Value)> f;
    public:
        FunctionForkBehaviour(std::function<Value(Value)> f);

        Block & chooseParent(const Blockchain & chain, const Miner & miner) const override;
        // Block & publish(const Blockchain & chain, const & miner) const;
        Value collectFees(const Blockchain & chain, const Miner & miner, const Block & block, Value txFeesAvailable) const override;
        Value payForward(const Blockchain & chain, const Miner & miner, const Block & block, Value fees) const override;

        bool shouldUndercut(const Blockchain & chain) const;
        bool valUnder(const Blockchain & chain) const;
        bool valCont(const Blockchain & chain) const;
        
        static std::function<Value(Value)> forkWithCoefficient(int coefficient);
    };
}

#endif
