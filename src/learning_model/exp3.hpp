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

    class Exp3 {
    private:
        std::vector<std::unique_ptr<Strategy>> strategies;
        std::vector<size_t> playerStrategies;
        std::vector<std::vector<StratWeight>> playersWeights;
        std::vector<std::vector<double>> playersProbabilities;

        const double phi;
        const size_t numStrategies;
        const size_t numPlayers;

        std::vector<double> probabilitiesForPlayer(size_t player);

    public:
        Exp3(
            std::vector<std::unique_ptr<Strategy>>& strategies,
            size_t numPlayers,
            double phi
        );

        // NOTE: following is api without tests
        void pickNewStrategies(size_t numPlayers);
        void updateWeights(std::vector<Value> rewards, Value maxReward);
        StratWeight getStrategyWeight(size_t strategyIndex) const;
    };
}

#endif
