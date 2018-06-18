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
        _blocksMinedTotal = BlockCount(0);
        totalMiningCost = 0;
    }

    void Miner::reset(Blockchain &chain) {
        _blocksMinedTotal = BlockCount(0);
        unbroadcastBlocks.clear();
        // _nextPublishTime = BlockTime(std::numeric_limits<TimeType>::max());
        // _lastCostUpdate = BlockTime(0);

        // no-one should mistakenly mine immediately the chain starts!
        _nextMiningTime = BlockTime(1) + utils::selectMiningOffset(chain.chanceToWin(params.hashRate));
        totalMiningCost = 0;
        // waitingForBroadcast = false;
    }

    void Miner::changeStrategy(LM::Strategy &strategy) {

    }

    std::unique_ptr<Block> Miner::mine(Blockchain &chain, BlockTime minedAt) {
        Block &parent = strategy.behaviour->chooseParent(chain, *this);

        BlockTime timeDiff = minedAt - parent.params.minedAt;
        Value txFeesAvailable = chain.txPooled(timeDiff) + parent.params.rem + parent.params.payForward;
        Value txFees = strategy.behaviour->collectFees(chain, *this, parent, txFeesAvailable);

        // this should maybe be in the strategy
        // whenToMine and chooseParent

        _blocksMinedTotal++;
        _nextMiningTime += utils::selectMiningOffset(chain.chanceToWin(params.hashRate));

        assert(minedAt > parent.params.minedAt);

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

        return chain.createBlock(&parent, this, params);
    }

    std::ostream& operator<<(std::ostream& os, const Miner& miner) {
        miner.print(os);
        return os;
    }

    void Miner::print(std::ostream& os) const {
        os << "[" << strategy.name  << "] miner " << params.name;
    }

}
