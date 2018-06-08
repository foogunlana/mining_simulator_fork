//
//  game_result.hpp
//
//

#ifndef game_result_hpp
#define game_result_hpp

#include "src/utils/typeDefs.hpp"
#include "miner_result.hpp"


#include <vector>


namespace mining_game {

    struct GameResult {
        std::vector<MinerResult> minerResults;
        BlockCount totalBlocksMined;
        BlockCount blocksInLongestChain;
        Value moneyLeftAtEnd;
        Value moneyInLongestChain;

        GameResult(std::vector<MinerResult> minerResults, BlockCount totalBlocksMined, BlockCount blocksInLongestChain, Value moneyLeftAtEnd, Value moneyInLongestChain);
        GameResult();
    };
}


#endif /* game_result_hpp */
