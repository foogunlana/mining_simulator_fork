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
        strategy(strategy_), blockchain(nullptr), params(params_)
    {
        unbroadcastBlocks.clear();
        _nextMiningTime = maxTime;
        totalMiningCost = 0;
    }

    void Miner::workOn(Blockchain *blockchain_) {
        blockchain = blockchain_;
        // _nextMiningTime = strategy.nextMiningTime(*this);
        _nextMiningTime = blockchain->getTime() + BlockTime(1) + (std::rand() % 100) / 100.0;
    }

    BlockTime Miner::nextMiningTime() const {
        // return strategy.nextMiningTime(*this);
        return _nextMiningTime;
    }

    std::unique_ptr<Block> Miner::mine(BlockTime finish) {
        // this should maybe be in the strategy
        _nextMiningTime = finish + BlockTime(1) + utils::selectMiningOffset(blockchain->chanceToWin(params.hashRate));
        Block *parent = blockchain->frontier()[0].get();
        return std::make_unique<Block>(parent);
    }
}
