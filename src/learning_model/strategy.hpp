//
//  strategy.hpp
//  learning_model
//

#ifndef strategy_hpp
#define strategy_hpp

#include "src/utils/typeDefs.hpp"

#include <string>

namespace learning_model {

    class Behaviour {
    public:
        Behaviour();
    };

    class Strategy {
    public:
        std::string name;
        StratWeight weight;
        const Behaviour *behaviour;

        Strategy(std::string name, StratWeight weight, const Behaviour* behaviour);
        Strategy(std::string name, StratWeight weight);
    };
}

#endif /* strategy_hpp */
