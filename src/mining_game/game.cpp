//
//  game.cpp
//
//

#include "miner_group.hpp"
#include "game.hpp"
#include "game_result.hpp"
#include "blockchain.hpp"
#include "blockchain_settings.hpp"

#include <iostream>

namespace mining_game {

    Game::Game(GameSettings settings_): blockchainSettings(settings_.blockchainSettings) {}

    GameResult Game::run(MinerGroup &minerGroup, Blockchain &blockchain) {

        BlockTime endTime = blockchainSettings.numberOfBlocks * blockchainSettings.secondsPerBlock;
        BlockTime currentTime, nextEventTime;

        minerGroup.workOn(blockchain);

        do {
            currentTime = blockchain.getTime();
            nextEventTime = minerGroup.nextEventTime();
            std::cout << nextEventTime << std::endl;
            // blockchain.advanceToTime(nextEventTime);


        } while (nextEventTime < endTime);

        return GameResult();
    }
}
