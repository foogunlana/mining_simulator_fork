#include "exp3.hpp"
#include "strategy.hpp"

#include <sstream>
#include <vector>
#include <assert.h>

namespace learning_model {

    Exp3::Exp3(
        std::vector<std::unique_ptr<Strategy>> &strategies_,
        size_t numPlayers_,
        double phi_) :
        strategies(std::move(strategies_)),
        phi(phi_),
        numStrategies(strategies_.size()),
        numPlayers(numPlayers_)
    {
        assert(strategies.size() > 0);

        playerStrategies.resize(numPlayers);
        playerWeights.reserve(numPlayers);
        playerProbabilities.resize(numPlayers);

        StratWeight totalWeight(0);
        for (auto &strategy : strategies) {
            totalWeight += strategy->weight;
        }

        for (size_t i = 0; i < strategies.size(); i++) {
            strategies[i]->weight /= totalWeight;
        }

        std::vector<StratWeight> weights;
        weights.reserve(numStrategies);
        std::transform(
            std::begin(strategies),
            std::end(strategies),
            std::back_inserter(weights),
            [](const auto &s) { return s->weight; });

        for (size_t i = 0; i < numPlayers; i++) {
            playerWeights.push_back(weights);
        }
    }

    void pickNewStrategies(double phi, size_t numPlayers) {

    }

    void updateWeights(std::vector<Value> rewards, Value maxReward, double phi) {

    }

    void getWeightsForPlayer() {

    }

    void printWeights() {

    }
}
