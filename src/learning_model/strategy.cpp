//
//  strategy.hpp
//  learning_model
//

#include "src/learning_model/strategy.hpp"
#include "src/utils/typeDefs.hpp"
#include "src/strategy_behaviour/behaviour.hpp"

#include <string>


namespace learning_model {
    Strategy::Strategy(std::string name_, StratWeight weight_, const SB::Behaviour *behaviour_):
        name(name_), weight(weight_), behaviour(behaviour_) {}
}
