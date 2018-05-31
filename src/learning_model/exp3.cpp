#include "exp3.hpp"
#include "strategy.hpp"

#include <sstream>
#include <vector>
#include <assert.h>
#include <math.h>
#include <random>
#include <iostream>
#include <typeinfo>

namespace learning_model {

    Exp3::Exp3(
        std::vector<std::unique_ptr<Strategy>> &strategies_,
        size_t numPlayers_,
        double phi_) :
        strategies(std::move(strategies_)),
        phi(phi_),
        numStrategies(strategies.size()),
        numPlayers(numPlayers_)
    {
        assert(strategies.size() > 0);

        playerStrategies.resize(numPlayers);
        playersWeights.reserve(numPlayers);
        playersProbabilities.resize(numPlayers);

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
            playersWeights.push_back(weights);
        }
    }

    void Exp3::pickNewStrategies(size_t numPlayers) {
        static std::random_device *rd = new std::random_device();
        static std::mt19937 gen((*rd)());

        for (size_t player = 0; player < numPlayers; player++) {
            std::vector<double> probabilities = probabilitiesForPlayer(player);
            std::discrete_distribution<std::size_t> dis(begin(probabilities), end(probabilities));
            size_t stratIndex = dis(gen);
            playerStrategies[player] = stratIndex;
        }
    }

    std::vector<double> Exp3::probabilitiesForPlayer(size_t player) {
        std::vector<double> probabilities;
        probabilities.reserve(numStrategies);

        for (size_t strategyIndex = 0; strategyIndex < numStrategies; strategyIndex++) {
            double probability = (1 - phi) * rawWeight(playersWeights[player][strategyIndex]) + phi / numStrategies;

            assert(probability > 0);
            assert(!isnan(probability) && !isinf(probability));
            assert(isfinite(probability) && isnormal(probability));

            probabilities.push_back(probability);
        }
        playersProbabilities[player] = probabilities;

        return playersProbabilities[player];
    }

    void Exp3::updateWeights(std::vector<Value> rewards, Value maxPossibleReward) {

        for (size_t player = 0; player < numPlayers; player++) {
            double rewardRatio = valuePercentage(rewards[player], maxPossibleReward);
            double normalizedReward(fmin(rewardRatio, 1.0));
            double gHat = normalizedReward / playersProbabilities[player][playerStrategies[player]];
            double weightAdjustment = exp((phi * gHat) / numStrategies);

            StratWeight oldWeight = playersWeights[player][playerStrategies[player]];
            playersWeights[player][playerStrategies[player]] *= StratWeight(weightAdjustment);
            StratWeight maxWeight = StratWeight(1) - oldWeight + playersWeights[player][playerStrategies[player]];

            for (size_t i = 0; i < numStrategies; i++) {
                playersWeights[player][i] /= maxWeight;
            }
        }

        std::vector<StratWeight> weights;
        weights.resize(numStrategies);

        for (auto &playerWeights : playersWeights) {
            for (size_t i = 0; i < numStrategies; i++) {
                weights[i] += playerWeights[i];
            }
        }

        for (size_t i = 0; i < numPlayers; i++) {
            strategies[i]->weight = weights[i] / StratWeight(numPlayers);
        }
    }

    StratWeight Exp3::getStrategyWeight(size_t i) const {
        return strategies.at(i)->weight;
    }

}
