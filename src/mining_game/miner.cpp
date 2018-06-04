//
//  miner.cpp
//
//

#include "miner.hpp"

#include <string>

namespace mining_game {

    Miner::Miner(MinerParameters params_, const std::string strategy_) :
        strategy(strategy_), params(params_) {}
}
