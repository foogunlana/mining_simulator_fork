#ifndef game_hpp
#define game_hpp

#include "src/utils/typeDefs.hpp"
#include "game_result.hpp"
#include "blockchain_settings.hpp"

namespace mining_game {

    struct GameSettings {
        BlockchainSettings blockchainSettings;
        bool commentary;
        std::vector<std::pair<std::string, double>> strategies;
    };

    class MinerGroup;

    class Game {
    private:
        BlockchainSettings blockchainSettings;
        std::vector<std::pair<std::string, double>> strategies;
        bool commentaryOn;
    public:
        Game(GameSettings settings);
        GameResult run(MinerGroup &minerGroup, Blockchain &blockchain);
    };
}

#endif
