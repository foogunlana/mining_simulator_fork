//
//  game.hpp
//  BlockSim
//
//

#ifndef game_hpp
#define game_hpp

#include "src/utils/typeDefs.hpp"
#include "game_result.hpp"
#include "blockchain_settings.hpp"

namespace mining_game {

    struct GameSettings {
        BlockchainSettings blockchainSettings;
    };

    class MinerGroup;

    class Game {
    private:
        BlockchainSettings blockchainSettings;
    public:
        Game(GameSettings settings);
        GameResult run(MinerGroup &minerGroup, Blockchain &blockchain);
    };
}

#endif /* game_hpp */
