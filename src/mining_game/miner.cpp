//
//  miner.cpp
//
//

#include "miner.hpp"
#include "src/utils/utils.hpp"

#include "blockchain.hpp"
#include "block.hpp"

#include <string>
#include <random>

constexpr auto maxTime = BlockTime(std::numeric_limits<BlockTime>::max());

namespace mining_game {

    Miner::Miner(MinerParameters params_, const std::string strategy_) :
        strategy(strategy_), params(params_)
    {
        _nextMiningTime = maxTime;
        totalMiningCost = 0;
    }

    void Miner::start(const Blockchain & chain) {
        _nextMiningTime = chain.getTime() + BlockTime(1) + utils::selectMiningOffset(chain.chanceToWin(params.hashRate));
    }

    BlockTime Miner::nextMiningTime() const {
        // return strategy.nextMiningTime(*this);
        return _nextMiningTime;
    }

    std::unique_ptr<Block> Miner::mine(Blockchain &chain) {
        // this should maybe be in the strategy
        // whenToMine and chooseParent
        _nextMiningTime += BlockTime(1) + utils::selectMiningOffset(chain.chanceToWin(params.hashRate));
        Block *parent = chain.frontier()[0].get();
        return std::make_unique<Block>(parent);
    }
}
