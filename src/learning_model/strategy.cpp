//
//  strategy.hpp
//  learning_model
//

#include "src/learning_model/strategy.hpp"
#include "src/utils/typeDefs.hpp"

#include <string>

namespace learning_model {
    Strategy::Strategy(std::string name_, StratWeight weight_, const Behaviour *behaviour_):
        name(name_), weight(weight_), behaviour(behaviour_) {}

    Behaviour::Behaviour() {}
}
