//
//  strategy.hpp
//  learning_model
//

#ifndef strategy_hpp
#define strategy_hpp

#include "src/utils/typeDefs.hpp"

#include <string>

namespace learning_model {

    class Strategy {
    public:
        std::string name;
        StratWeight weight;

        Strategy(std::string name, StratWeight weight);
    };
}

#endif /* strategy_hpp */
