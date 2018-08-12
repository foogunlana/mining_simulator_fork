#include "miner_result.hpp"
#include "block.hpp"

#include <iostream>

namespace mining_game {

    MinerResult::MinerResult() : blocksInWinningChain(BlockCount(0)), totalProfit(Profit(0)) {}

    void MinerResult::addBlock(const Block *block) {
        totalProfit += block->realValue();
        blocksInWinningChain++;
    }
}
