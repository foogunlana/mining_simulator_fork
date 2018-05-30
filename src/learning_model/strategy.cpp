//
//  strategy.hpp
//  learning_model
//

#include "src/learning_model/strategy.hpp"
#include "src/utils/typeDefs.hpp"

#include <string>

namespace learning_model {
    Strategy::Strategy(std::string name_, StratWeight weight_): name(name_), weight(weight_) {}
}
