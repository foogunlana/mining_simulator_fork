//
//  exp3.hpp
//  learning_model
//

#ifndef exp3_hpp
#define exp3_hpp

#include "src/utils/typeDefs.hpp"

#include <vector>
#include <string>
#include <stdio.h>

namespace learning_model {

    class Strategy;

    struct PlayerProfile {
        std::vector<StratWeight> weights;
        std::vector<double> probabilities;
        size_t currentStrategy;
        Value currentReward;
    };

    class Exp3 {
    private:
        std::vector<std::unique_ptr<Strategy>> strategies;
        const double phi;

        std::vector<double> probabilitiesFromWeights(const std::vector<StratWeight> &weights);
    public:
        Exp3(
            std::vector<std::unique_ptr<Strategy>>& strategies,
            double phi
        );

        std::vector<PlayerProfile> pickStrategiesEvenly(size_t numPlayers);
        std::vector<PlayerProfile> updateStrategyProfiles(const std::vector<PlayerProfile> &profiles, Value maxPossibleReward);

        std::vector<PlayerProfile> pickStrategiesWithWeights(const std::vector<std::vector<StratWeight>> &weights);
        std::vector<std::vector<StratWeight>> updateWeights(const std::vector<PlayerProfile> &profiles, Value maxPossibleReward);
        std::vector<StratWeight> getStrategyWeights() const;
    };
}

#endif
