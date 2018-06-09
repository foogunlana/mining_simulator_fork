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

    size_t strategyFromProbabilities(std::mt19937 &gen, std::vector<double> &probabilities);

    Exp3::Exp3(
        const std::vector<Strategy *> strategies_,
        double phi_) :
        strategies(strategies_),
        phi(phi_)
    {
        assert(strategies.size() > 0);

        StratWeight totalWeight(0);
        for (auto &strategy : strategies) {
            totalWeight += strategy->weight;
        }

        for (size_t i = 0; i < strategies.size(); i++) {
            strategies[i]->weight /= totalWeight;
        }
    }

    std::vector<PlayerProfile> Exp3::pickStrategiesEvenly(size_t numPlayers) const {
        std::vector<std::vector<StratWeight>> playerWeights;
        playerWeights.resize(numPlayers);
        for (auto &weights : playerWeights) {
            for (auto &strategy : strategies) {
                weights.push_back(strategy->weight);
            }
        }
        return pickStrategiesWithWeights(playerWeights);
    }

    std::vector<PlayerProfile> Exp3::pickStrategiesWithWeights(
        const std::vector<std::vector<StratWeight>> &weights
    ) const {
        std::vector<PlayerProfile> profiles;
        static std::random_device *rd = new std::random_device();
        static std::mt19937 gen((*rd)());

        size_t numPlayers(weights.size());
        for (size_t i = 0; i < numPlayers; i++) {
            auto p = PlayerProfile();
            StratWeight totalWeight(0);
            for (auto w : weights[i]) {
                totalWeight += w;
            }
            for (auto weight : weights[i]) {
                p.weights.push_back(weight / totalWeight);
            }
            p.probabilities = probabilitiesFromWeights(p.weights);
            p.currentStrategy = strategyFromProbabilities(gen, p.probabilities);
            p.currentReward = 0;
            profiles.push_back(p);
        }
        return profiles;
    }

    // for convenience
    std::vector<PlayerProfile> Exp3::updateStrategyProfiles(
        const std::vector<PlayerProfile> &playerProfiles,
        Value maxPossibleReward
    ) {
        auto updatedWeights = updateWeights(playerProfiles, maxPossibleReward);
        return pickStrategiesWithWeights(updatedWeights);
    }

    std::vector<std::vector<StratWeight>> Exp3::updateWeights(
        const std::vector<PlayerProfile> &playerProfiles,
        Value maxPossibleReward
    ) {
        size_t numStrategies = strategies.size();
        size_t numPlayers = playerProfiles.size();

        std::vector<std::vector<StratWeight>> newWeights;

        // NOTE: Maybe throw an error if the max reward is less than any reward
        for (auto &player : playerProfiles) {
            double rewardRatio = valuePercentage(player.currentReward, maxPossibleReward);
            double normalizedReward(fmin(rewardRatio, 1.0));
            double gHat = normalizedReward / player.probabilities[player.currentStrategy];
            double weightAdjustment = exp((phi * gHat) / numStrategies);

            std::vector<StratWeight> playerWeights(player.weights);

            StratWeight oldWeight = playerWeights[player.currentStrategy];
            playerWeights[player.currentStrategy] *= StratWeight(weightAdjustment);
            StratWeight maxWeight = StratWeight(1) - oldWeight + playerWeights[player.currentStrategy];

            for (size_t i = 0; i < numStrategies; i++) {
                playerWeights[i] /= maxWeight;
            }
            newWeights.push_back(playerWeights);
        }

        std::vector<StratWeight> updatedWeights;
        updatedWeights.resize(numStrategies);

        for (size_t strategy = 0; strategy < numStrategies; strategy++) {
            for (auto &playerWeights : newWeights) {
                updatedWeights[strategy] += playerWeights[strategy];
            }
            updatedWeights[strategy] /= StratWeight(numPlayers);
            strategies[strategy]->weight = updatedWeights[strategy];
        }
        return newWeights;
    }

    std::vector<StratWeight> Exp3::getStrategyWeights() const {
        std::vector<StratWeight> weights;
        for (auto &s : strategies) {
            weights.push_back(s->weight);
        }
        return weights;
    }

    std::vector<double> Exp3::probabilitiesFromWeights(const std::vector<StratWeight> &weights) const {
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

    size_t strategyFromProbabilities(std::mt19937 &gen, std::vector<double> &probabilities) {
        std::discrete_distribution<std::size_t> dis(begin(probabilities), end(probabilities));
        size_t stratIndex = dis(gen);
        return stratIndex;
    }
}
