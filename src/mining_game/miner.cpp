//
//  miner.cpp
//
//

#include "miner.hpp"
#include "src/utils/utils.hpp"
#include "src/learning_model/strategy.hpp"
#include "src/learning_model/behaviour.hpp"

#include "blockchain.hpp"
#include "block.hpp"

#include <string>
#include <random>
#include <iostream>
#include <cassert>

// constexpr auto maxTime = BlockTime(std::numeric_limits<BlockTime>::max());

namespace LM = learning_model;

namespace mining_game {

    Miner::Miner(MinerParameters params_, LM::Strategy &strategy_, const Blockchain &chain) :
        strategy(strategy_), params(params_)
    {
        _nextMiningTime = utils::selectMiningOffset(chain.chanceToWin(params.hashRate));
        totalMiningCost = 0;
    }

    void Miner::changeStrategy(LM::Strategy &strategy) {

    }

    std::unique_ptr<Block> Miner::mine(Blockchain &chain, BlockTime minedAt) {
        Block &parent = strategy.behaviour->chooseParent(chain, *this);

        // this should maybe be in the strategy
        // whenToMine and chooseParent

        _nextMiningTime += utils::selectMiningOffset(chain.chanceToWin(params.hashRate));

        assert(minedAt > parent.minedAt);

        BlockTime timeDiff = minedAt - parent.minedAt;
        Value txFeesAvailable = chain.txPooled(timeDiff) + parent.surplus;
        Value txFees = txFeesAvailable;
        Value surplus = txFeesAvailable - txFees;

        // need to mock out reference to behaviour from strategy to test!!!
        // else, can't make more complex blocks as the behaviour must do it too
        return std::make_unique<Block>(&parent, this, minedAt, txFees, surplus);
    }
}
