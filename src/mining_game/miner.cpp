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

constexpr auto maxTime = BlockTime(std::numeric_limits<BlockTime>::max());

namespace LM = learning_model;

namespace mining_game {

    Miner::Miner(MinerParameters params_, LM::Strategy &strategy_) :
        strategy(strategy_), params(params_)
    {
        _nextMiningTime = maxTime;
        totalMiningCost = 0;
    }

    void Miner::start(const Blockchain & chain) {
        _nextMiningTime = chain.getTime() + BlockTime(1) + utils::selectMiningOffset(chain.chanceToWin(params.hashRate));
    }

    void Miner::changeStrategy(LM::Strategy &strategy) {

    }

    std::unique_ptr<Block> Miner::mine(Blockchain &chain) {
        // this should maybe be in the strategy
        // whenToMine and chooseParent
        _nextMiningTime += BlockTime(1) + utils::selectMiningOffset(chain.chanceToWin(params.hashRate));
        // Block *parent = chain.frontier()[0].get();
        Block &parent = strategy.behaviour.chooseParent(chain, *this);
        // std::cout <<
        //     "miner" << " " << params.name << " " <<
        //     "mined on miner" << parent.miner.params.name <<
        //     "'s block at time " << chain.getTime() << '\n';

        // need to mock out reference to behaviour from strategy to test!!!
        // else, can't make more complex blocks as the behaviour must do it too
        return std::make_unique<Block>(&parent);
    }
}
