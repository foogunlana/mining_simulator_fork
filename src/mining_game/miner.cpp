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

    void Miner::changeStrategy(LM::Strategy &newStrategy) {
        strategy = newStrategy;
    }

    std::unique_ptr<Block> Miner::mine(Blockchain &chain, BlockTime now) {
        Block &parent = strategy.get().behaviour->chooseParent(chain, *this);

        // chain.rem includes the remainder from parent, pay forward and any tx inbetween until 'now'
        Value txFeesAvailable = chain.rem(parent, now);
        Value txFees = strategy.get().behaviour->collectFees(chain, *this, parent, txFeesAvailable);

        _blocksMinedTotal++;
        _nextMiningTime += utils::selectMiningOffset(chain.chanceToWin(params.hashRate));

        assert(now > parent.params.minedAt);

        Value rem = txFeesAvailable - txFees;
        Value payForward = Value(0);
        BlockTime publishedAt = now;
        BlockTime minedAt = now;

        auto blockParams = BlockParameters{
            minedAt,
            publishedAt,
            txFees,
            parent.params.fixedReward,
            rem,
            payForward
        };
        return chain.createBlock(&parent, this, blockParams);
    }

    std::ostream& operator<<(std::ostream& os, const Miner& miner) {
        miner.print(os);
        return os;
    }

    void Miner::print(std::ostream& os) const {
        os << "[" << strategy.get().name  << "] miner " << params.name;
    }

}
