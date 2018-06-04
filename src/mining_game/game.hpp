//
//  game.hpp
//  BlockSim
//
//

#ifndef game_hpp
#define game_hpp

#include "src/utils/typeDefs.hpp"
#include "game_result.hpp"
#include "miner_group.hpp"

namespace mining_game {

    struct BlockchainSettings {
        BlockRate secondsPerBlock;
        ValueRate transactionFeeRate;
        BlockValue blockReward;
        BlockCount numberOfBlocks;
    };

    struct GameSettings {
        BlockchainSettings blockchainSettings;
    };

    class Game {
    private:
        GameSettings settings;
    public:
        Game(GameSettings settings);
        GameResult run(MinerGroup &minerGroup);
    };
}

#endif /* game_hpp */
