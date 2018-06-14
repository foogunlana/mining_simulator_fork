//
//  miner_result.cpp
//
//

#include "miner_result.hpp"
#include "block.hpp"

namespace mining_game {

    MinerResult::MinerResult() : blocksInWinningChain(BlockCount(0)), totalProfit(Value(0)) {}

    void MinerResult::addBlock(const Block *block) {
        totalProfit += block->realValue();
        blocksInWinningChain++;
    }
}
