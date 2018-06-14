//
//  miner_result.hpp
//
//

#ifndef miner_result_hpp
#define miner_result_hpp

#include "src/utils/typeDefs.hpp"


namespace mining_game {

    class Block;

    struct MinerResult {
        BlockCount blocksInWinningChain;
        Profit totalProfit;

        MinerResult();
        void addBlock(const Block *block);
    };
}

#endif /* miner_result_hpp */
