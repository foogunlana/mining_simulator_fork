#ifndef strategy_hpp
#define strategy_hpp

#include "src/utils/typeDefs.hpp"
#include "src/strategy_behaviour/behaviour.hpp"

#include <string>

namespace SB = strategy_behaviour;

namespace learning_model {

    class Strategy {
    public:
        std::string name;
        StratWeight weight;
        const SB::Behaviour *behaviour;

        Strategy(std::string name, StratWeight weight, const SB::Behaviour *behaviour);
    };
}

#endif
