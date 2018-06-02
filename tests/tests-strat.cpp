#include "lib/catch/catch.hpp"
#include "src/learning_model/exp3.hpp"
#include "src/learning_model/strategy.hpp"

#include <vector>
#include <string>

namespace LM = learning_model;

SCENARIO("Exp3 learning model") {
    double weight1 = 4;
    double weight2 = 1;
    auto s1 = std::make_unique<LM::Strategy>("strategy1", weight1);
    auto s2 = std::make_unique<LM::Strategy>("strategy2", weight2);

    std::vector<std::unique_ptr<LM::Strategy>> strategies;
    strategies.push_back(std::move(s1));
    strategies.push_back(std::move(s2));
    size_t numStrategies = strategies.size();
    double phi(.01);

    GIVEN("strategies picked somehow") {

        LM::Exp3 model(strategies, phi);
        size_t numPlayers = 4;
        std::vector<LM::PlayerProfile> profiles = model.pickStrategiesEvenly(numPlayers);

        // NOTE: should test randomness of strategy picking somehow...

        THEN("returns player profiles with no rewards") {
            for (size_t i = 0; i < numPlayers - 1; i++) {
                REQUIRE(profiles[i].currentReward == 0);
            }
        }

        THEN("returns players, each with some strategy") {
            for (size_t i = 0; i < numPlayers - 1; i++) {
                REQUIRE(profiles[i].currentStrategy < numStrategies);
            }
        }
    }

    GIVEN("strategies picked evenly [pickStrategiesEvenly]") {

        LM::Exp3 model(strategies, phi);
        size_t numPlayers = 4;
        std::vector<LM::PlayerProfile> profiles = model.pickStrategiesEvenly(numPlayers);

        THEN("returns correct number of player profiles") {
            REQUIRE(profiles.size() == numPlayers);
        }

        THEN("results in original normalised weights") {
            double total = weight1 + weight2;
            for (size_t i = 0; i < numPlayers - 1; i++) {
                CHECK(profiles[i].weights[0] == weight1 / total);
                CHECK(profiles[i].weights[1] == weight2 / total);
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

    SECTION("updating weights") {
        GIVEN("weights start off equal") {
            auto s1 = std::make_unique<LM::Strategy>("strategy1", weight1);
            auto s2 = std::make_unique<LM::Strategy>("strategy2", weight1);

            std::vector<std::unique_ptr<LM::Strategy>> strategies2;
            strategies2.push_back(std::move(s1));
            strategies2.push_back(std::move(s2));
            double phi(.01);
            LM::Exp3 model2(strategies2, phi);

        }
    }
}
