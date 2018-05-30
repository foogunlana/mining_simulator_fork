#include "lib/catch/catch.hpp"
#include "src/learning_model/exp3.hpp"
#include "src/learning_model/strategy.hpp"

#include <vector>
#include <string>

namespace LM = learning_model;

SCENARIO("Exp3 learning model") {
    GIVEN("Exp3 is initialized") {
        std::vector<std::unique_ptr<LM::Strategy>> strategies;

        double weight = 0.2;
        auto s1 = std::make_unique<LM::Strategy>("strategy1", weight);
        auto s2 = std::make_unique<LM::Strategy>("strategy2", weight);

        strategies.push_back(std::move(s1));
        strategies.push_back(std::move(s2));
        size_t numPlayers(10);
        double phi(.01);

        LM::Exp3 model(strategies, numPlayers, phi);

        WHEN("getting weights vector for player") {
            std::vector<double> weights = model.getWeightsForPlayer();

            THEN("weights are normalised and initialized") {
                REQUIRE(weights.at(0) == 0.5);
                REQUIRE(weights.at(1) == 0.5);
            }
        }
    }
}
