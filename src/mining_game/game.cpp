//
//  game.cpp
//
//

#include "miner_group.hpp"
#include "game.hpp"
#include "game_result.hpp"
#include "blockchain.hpp"
#include "blockchain_settings.hpp"


namespace mining_game {

    Game::Game(GameSettings settings_): blockchainSettings(settings_.blockchainSettings) {}

    GameResult Game::run(MinerGroup &minerGroup) {

        Blockchain blockchain = Blockchain(blockchainSettings);
        BlockTime duration = blockchainSettings.numberOfBlocks * blockchainSettings.secondsPerBlock;

        BlockTime currentTime, nextEventTime;

        do {
            currentTime = blockchain.getTime();
            nextEventTime = minerGroup.nextEventTime(currentTime);
            blockchain.advanceToTime(nextEventTime);

            
        } while (nextEventTime < duration);

        return GameResult();
    }
}
