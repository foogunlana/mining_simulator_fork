
#ifndef player_profile_hpp
#define player_profile_hpp

#include "src/utils/typeDefs.hpp"

#include <vector>

namespace learning_model {

    struct PlayerProfile {
        std::vector<StratWeight> weights;
        std::vector<double> probabilities;
        size_t currentStrategy;
        Profit currentReward;
    };
}

#endif
