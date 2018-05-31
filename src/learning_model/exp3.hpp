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
        std::vector<std::vector<StratWeight>> playersWeights;
        
        const double phi;

        std::vector<double> probabilitiesForPlayer(size_t player);

    public:
        Exp3(
            std::vector<std::unique_ptr<Strategy>>& strategies,
            size_t numPlayers,
            double phi
        );

        // NOTE: following is api without tests
        std::vector<size_t> pickNewStrategies(std::vector<std::vector<double>> &probabilities);
        std::vector<double> weightsToProbabilities(const std::vector<StratWeight> &weights);
        void updateWeights(
            size_t numPlayers,
            const std::vector<std::vector<StratWeight>> &oldWeights,
            const std::vector<std::vector<double>> &oldProbabilities,
            const std::vector<size_t> &oldStrategies,
            const std::vector<Value> &rewards,
            Value maxPossibleReward);
        StratWeight getStrategyWeight(size_t strategyIndex) const;
    };
}

#endif
