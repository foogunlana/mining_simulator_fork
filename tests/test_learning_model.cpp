#include "lib/catch/catch.hpp"
#include "src/learning_model/exp3.hpp"
#include "src/learning_model/strategy.hpp"
#include "src/learning_model/player_profile.hpp"

#include <vector>
#include <map>
#include <string>
#include <random>
#include <iostream>

namespace LM = learning_model;

SCENARIO("Exp3 learning model") {

    double weight1(4);
    double weight2(1);
    double weight3(5);
    auto placeholderBehaviour = LM::Behaviour();

    auto s1(std::make_unique<LM::Strategy>("strategy1", weight1, placeholderBehaviour));
    auto s2(std::make_unique<LM::Strategy>("strategy2", weight2, placeholderBehaviour));
    auto s3(std::make_unique<LM::Strategy>("strategy2", weight3, placeholderBehaviour));

    StratWeight total(0);

    std::vector<LM::Strategy *> strategies;
    strategies.push_back(s1.get());
    strategies.push_back(s2.get());
    strategies.push_back(s3.get());
    for (auto &s : strategies) {
        total += s->weight;
    }

    size_t numStrategies = strategies.size();
    double phi(.01);

    GIVEN("strategies picked somehow [pickStrategies*]") {

        LM::Exp3 model(strategies, phi);
        size_t numPlayers(4);
        std::vector<std::vector<StratWeight>> weights;
        weights.resize(numPlayers);
        for (auto &playerWeights : weights) {
            for (size_t i = 0; i < numStrategies; i++) {
                playerWeights.push_back(std::rand() % 10);
            }
        }
        std::vector<StratWeight> total2;
        total2.resize(numPlayers);
        for (size_t i = 0; i < numPlayers; i++) {
            for (size_t j = 0; j < numStrategies; j++) {
                total2[i] += weights[i][j];
            }
        }
        std::vector<LM::PlayerProfile> profiles = model.pickStrategiesEvenly(numPlayers);
        std::vector<LM::PlayerProfile> profiles2 = model.pickStrategiesWithWeights(weights);

        // NOTE: should test randomness of strategy picking somehow...
        // NOTE: should test that reward is zero after updating strategies

        THEN("returns player profiles with no rewards") {
            for (size_t i = 0; i < numPlayers; i++) {
                REQUIRE(profiles[i].currentReward == 0);
            }
            for (size_t i = 0; i < numPlayers; i++) {
                REQUIRE(profiles2[i].currentReward == 0);
            }
        }

        THEN("returns players, each with some strategy") {
            for (size_t i = 0; i < numPlayers; i++) {
                REQUIRE(profiles[i].currentStrategy < numStrategies);
            }
            for (size_t i = 0; i < numPlayers; i++) {
                REQUIRE(profiles2[i].currentStrategy < numStrategies);
            }
        }

        THEN("returns correct number of player profiles") {
            REQUIRE(profiles.size() == numPlayers);
            REQUIRE(profiles2.size() == numPlayers);
        }

        THEN("results in original weights but normalised") {
            for (size_t i = 0; i < numPlayers; i++) {
                CHECK(profiles[i].weights[0] == weight1 / total);
                CHECK(profiles[i].weights[1] == weight2 / total);
                CHECK(profiles[i].weights[2] == weight3 / total);
                for (size_t j = 0; j < numStrategies; j++) {
                    REQUIRE(profiles2[i].weights[j] == weights[i][j] / total2[i]);
                }
            }
        }
    }

    GIVEN("strategies picked evenly [pickStrategiesEvenly]") {

        LM::Exp3 model(strategies, phi);
        size_t numPlayers = 10000;
        std::vector<LM::PlayerProfile> profiles = model.pickStrategiesEvenly(numPlayers);

        THEN("uses all strategies") {
            size_t numStrategies(strategies.size());
            std::vector<bool> used(numStrategies);
            std::fill(used.begin(), used.end(), false);

            for (size_t i = 0; i < numPlayers - 1; i++) {
                used[profiles[i].currentStrategy] = true;
            }
            REQUIRE(used[0]);
            REQUIRE(used[1]);
            REQUIRE(used[2]);
        }

        THEN("strategies are not all the same") {
            size_t numStrategies(strategies.size());
            std::vector<bool> used(numStrategies);
            std::fill(used.begin(), used.end(), false);

            bool same(true);
            for (size_t i = 0; i < numPlayers - 1; i++) {
                same = same && (profiles[i].currentStrategy == profiles[i+1].currentStrategy);
                if (!same) break;
            }
            REQUIRE_FALSE(same);
        }

        THEN("strategies are allocated proportionally") {
            size_t numStrategies(strategies.size());
            std::map<size_t, unsigned int> used;

            double stratTotal(0);
            for (auto &s: strategies) {
                stratTotal += s->weight;
            }

            for (size_t i = 0; i < numPlayers - 1; i++) {
                used[profiles[i].currentStrategy] += 1;
            }

            for (size_t i = 0; i < numStrategies - 1; i++) {
                auto proportion = strategies[i]->weight/stratTotal;
                CHECK(proportion == Approx((double) used[i]/numPlayers).epsilon(0.1));
            }
        }

        THEN("results in equal weights") {
            for (size_t i = 0; i < numPlayers - 1; i++) {
                REQUIRE(profiles[i].weights == profiles[i+1].weights);
            }
        }

        THEN("results in equal strategy probabilities for each player") {
            for (size_t i = 0; i < numPlayers - 1; i++) {
                REQUIRE(profiles[i].probabilities == profiles[i+1].probabilities);
            }
        }
    }

    GIVEN("weights start off heavily (1 - 0) biased") {
        weight1 = 0;
        weight2 = 0;
        weight3 = 1;
        auto s1 = std::make_unique<LM::Strategy>("strategy1", weight1, placeholderBehaviour);
        auto s2 = std::make_unique<LM::Strategy>("strategy2", weight2, placeholderBehaviour);
        auto s3 = std::make_unique<LM::Strategy>("strategy3", weight3, placeholderBehaviour);

        std::vector<LM::Strategy *> strategies2;
        strategies2.push_back(s1.get());
        strategies2.push_back(s2.get());
        strategies2.push_back(s3.get());
        double phi(.01);
        LM::Exp3 model2(strategies2, phi);

        // WARNING: implement to assert with high probability, but not certainty
        THEN("only biased strategy is selected {MOST OF THE TIME}") {
            // size_t numPlayers = 5;
            // std::vector<LM::PlayerProfile> profiles = model2.pickStrategiesEvenly(numPlayers);
            // for (auto &player : profiles) {
            //     REQUIRE(player.currentStrategy == 2);
            // }
        }

        THEN("biased probability is approximately 1 and others are 0") {
            size_t numPlayers = 3;
            size_t biasedStrategy = 2;
            std::vector<LM::PlayerProfile> profiles = model2.pickStrategiesEvenly(numPlayers);
            for (auto &player : profiles) {
                for (size_t strategy = 0; strategy < strategies2.size(); strategy++) {
                    if (strategy == biasedStrategy) continue;
                    REQUIRE(player.probabilities[strategy]+1 == Approx(1).epsilon(0.01));
                    // NOTE: +1 because Catch 2 library uses % error for epsilon. 0.01 = 1% but 1% of 0 is 0!!!
                    // See https://github.com/catchorg/Catch2/blob/master/docs/assertions.md#epsilon-example
                }
                REQUIRE(player.probabilities[biasedStrategy] == Approx(1).epsilon(0.01));
            }
        }
    }

    SECTION("updating weights [updateWeights]") {
        double weight1(4);
        auto s1(std::make_unique<LM::Strategy>("strategy1", weight1, placeholderBehaviour));
        auto s2(std::make_unique<LM::Strategy>("strategy2", weight1, placeholderBehaviour));
        auto s3(std::make_unique<LM::Strategy>("strategy3", weight1, placeholderBehaviour));

        StratWeight total(0);

        std::vector<LM::Strategy *> strategies;
        strategies.push_back(s1.get());
        strategies.push_back(s2.get());
        strategies.push_back(s3.get());
        for (auto &s : strategies) {
            total += s->weight;
        }

        size_t numStrategies = strategies.size();
        double phi(.01);

        GIVEN("even strategies and biased rewards (only one player gets a reward with one strategy)") {
            LM::Exp3 model(strategies, phi);
            std::vector<LM::PlayerProfile> profiles = model.pickStrategiesEvenly(3);
            profiles[0].currentStrategy = 1;
            profiles[0].currentReward = Value(100);
            profiles[1].currentStrategy = 0;
            profiles[1].currentReward = Value(0);
            profiles[2].currentStrategy = 2;
            profiles[2].currentReward = Value(0);
            std::vector<StratWeight> originalWeights = model.getStrategyWeights();
            for (auto i = 0; i < originalWeights.size() - 1; i++) {
                CHECK(originalWeights[i] == originalWeights[i+1]);
            }

            THEN("result is biased updated strategy weights") {
                Value maxPossibleReward(Value(1));
                model.updateWeights(profiles, maxPossibleReward);
                std::vector<StratWeight> strategyWeights = model.getStrategyWeights();
                REQUIRE(strategyWeights[1] > strategyWeights[0]);
                REQUIRE(strategyWeights[1] > strategyWeights[2]);
            }

            THEN("returns biased player weights (only player with good strategy learns anything)") {
                Value maxPossibleReward(Value(1));
                std::vector<std::vector<StratWeight>> weights = model.updateWeights(
                    profiles, maxPossibleReward);

                size_t player(0);
                size_t strategy(1);
                // increased weights for strategy that was rewarded
                REQUIRE(weights[player][strategy] > weights[player][0]);
                REQUIRE(weights[player][strategy] > weights[player][2]);
                player++;
                // learned nothing
                REQUIRE(weights[player][strategy] == weights[player][0]);
                REQUIRE(weights[player][strategy] == weights[player][2]);
                player++;
                // learned nothing
                REQUIRE(weights[player][strategy] == weights[player][0]);
                REQUIRE(weights[player][strategy] == weights[player][2]);
            }
        }
    }
}
