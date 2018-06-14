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

        assert(minedAt > parent.params.minedAt);

        BlockTime timeDiff = minedAt - parent.params.minedAt;
        Value txFeesAvailable = chain.txPooled(timeDiff) + parent.params.rem + parent.params.payForward;
        Value txFees = txFeesAvailable;
        Value rem = txFeesAvailable - txFees;
        Value payForward = Value(0);
        BlockTime publishedAt = minedAt;

        auto params = BlockParameters{
            minedAt,
            publishedAt,
            parent.params.fixedReward,
            txFees,
            rem,
            payForward
        };

        return std::make_unique<Block>(&parent, this, params);
    }
}
