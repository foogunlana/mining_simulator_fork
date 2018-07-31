
#ifndef behaviour_hpp
#define behaviour_hpp

#include "src/utils/typeDefs.hpp"

namespace mining_game {
    class Blockchain;
    class Block;
    class Miner;
}

namespace MG = mining_game;

namespace strategy_behaviour {

    class Behaviour {
    public:
        Behaviour();
        virtual ~Behaviour() =default;

        virtual MG::Block & chooseParent(
            const MG::Blockchain & chain,
            const MG::Miner & miner
        ) const = 0;

        virtual Value collectFees(
            const MG::Blockchain & chain,
            const MG::Miner & miner,
            const MG::Block & block,
            Value txFeesAvailable
        ) const = 0;

        virtual Value payForward(
            const MG::Blockchain & chain,
            const MG::Miner & miner,
            const MG::Block & block,
            Value fees
        ) const = 0;
    };
}

#endif
