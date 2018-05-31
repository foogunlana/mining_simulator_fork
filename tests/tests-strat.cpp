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
    size_t numPlayers(2);
    double phi(.01);

    SECTION("Initialization") {

        LM::Exp3 model(strategies, numPlayers, phi);

        THEN("weights are normalised and initialized") {
            REQUIRE(model.getStrategyWeight(0) == 0.8);
            REQUIRE(model.getStrategyWeight(1) == 0.2);
        }
    }

    SECTION("updating weights") {
        GIVEN("weights start off equal") {
            auto s1 = std::make_unique<LM::Strategy>("strategy1", weight1);
            auto s2 = std::make_unique<LM::Strategy>("strategy2", weight1);

            std::vector<std::unique_ptr<LM::Strategy>> strategies2;
            strategies2.push_back(std::move(s1));
            strategies2.push_back(std::move(s2));
            size_t numPlayers(2);
            double phi(.01);
            LM::Exp3 model2(strategies2, numPlayers, phi);

            StratWeight weightA = model2.getStrategyWeight(0);
            StratWeight weightB = model2.getStrategyWeight(1);
            REQUIRE(model2.getStrategyWeight(0) == model2.getStrategyWeight(1));
        }
    }
}
