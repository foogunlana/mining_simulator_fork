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
        phi(phi_)
    {
        assert(strategies.size() > 0);
        playersWeights.reserve(numPlayers_);

        StratWeight totalWeight(0);
        for (auto &strategy : strategies) {
            totalWeight += strategy->weight;
        }

        for (size_t i = 0; i < strategies.size(); i++) {
            strategies[i]->weight /= totalWeight;
        }

        std::vector<StratWeight> weights;
        weights.reserve(strategies.size());
        std::transform(
            std::begin(strategies),
            std::end(strategies),
            std::back_inserter(weights),
            [](const auto &s) { return s->weight; });

        for (size_t i = 0; i < numPlayers_; i++) {
            playersWeights.push_back(weights);
        }
    }

    std::vector<double> Exp3::weightsToProbabilities(const std::vector<StratWeight> &weights) {
        std::vector<double> probabilities;
        size_t numStrategies = weights.size();
        assert(numStrategies != 0);
        probabilities.reserve(numStrategies);

        for (size_t strategyIndex = 0; strategyIndex < numStrategies; strategyIndex++) {
            double probability = (1 - phi) * rawWeight(weights[strategyIndex]) + phi / numStrategies;

            assert(probability > 0);
            assert(!isnan(probability) && !isinf(probability));
            assert(isfinite(probability) && isnormal(probability));

            probabilities.push_back(probability);
        }
        return probabilities;
    }

    std::vector<size_t> Exp3::pickNewStrategies(std::vector<std::vector<double>> &probabilities) {
        static std::random_device *rd = new std::random_device();
        static std::mt19937 gen((*rd)());

        size_t numPlayers = probabilities.size();
        assert(numPlayers > 0);
        std::vector<size_t> playerStrategies;

        playerStrategies.resize(numPlayers);

        for (size_t player = 0; player < numPlayers; player++) {
            std::discrete_distribution<std::size_t> dis(begin(probabilities[player]), end(probabilities[player]));
            size_t stratIndex = dis(gen);
            playerStrategies[player] = stratIndex;
        }
        return playerStrategies;
    }

    void Exp3::updateWeights(
        size_t numPlayers,
        const std::vector<std::vector<StratWeight>> &oldWeights,
        const std::vector<std::vector<double>> &oldProbabilities,
        const std::vector<size_t> &oldStrategies,
        const std::vector<Value> &rewards,
        Value maxPossibleReward
    ) {
        size_t numStrategies = oldStrategies.size();
        std::vector<std::vector<StratWeight>> newWeights;
        newWeights.resize(oldWeights.size());
        for(auto &weights : newWeights) {
            weights.resize(oldWeights[0].size());
        }

        for (size_t player = 0; player < numPlayers; player++) {
            double rewardRatio = valuePercentage(rewards[player], maxPossibleReward);
            double normalizedReward(fmin(rewardRatio, 1.0));
            double gHat = normalizedReward / oldProbabilities[player][oldStrategies[player]];
            double weightAdjustment = exp((phi * gHat) / numStrategies);


            StratWeight oldWeight = oldWeights[player][oldStrategies[player]];
            newWeights[player][oldStrategies[player]] *= StratWeight(weightAdjustment);
            StratWeight maxWeight = StratWeight(1) - oldWeight + newWeights[player][oldStrategies[player]];

            for (size_t i = 0; i < numStrategies; i++) {
                newWeights[player][i] /= maxWeight;
            }
        }

        std::vector<StratWeight> weights;
        weights.resize(numStrategies);

        for (auto &playerWeights : newWeights) {
            for (size_t i = 0; i < numStrategies; i++) {
                weights[i] += playerWeights[i];
            }
        }

        for (size_t i = 0; i < numPlayers; i++) {
            strategies[i]->weight = weights[i] / StratWeight(numPlayers);
        }
    }

    StratWeight Exp3::getStrategyWeight(size_t i) const {
        return strategies[i]->weight;
    }

}
