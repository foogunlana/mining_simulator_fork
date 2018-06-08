//
//  game.cpp
//
//

#include "miner_group.hpp"
#include "game.hpp"
#include "game_result.hpp"
#include "blockchain.hpp"
#include "blockchain_settings.hpp"
#include "block.hpp"


#include <iostream>

namespace mining_game {

    Game::Game(GameSettings settings_): blockchainSettings(settings_.blockchainSettings) {}

    GameResult Game::run(MinerGroup &minerGroup, Blockchain &blockchain) {

        BlockTime endTime = blockchainSettings.numberOfBlocks * blockchainSettings.secondsPerBlock;
        BlockTime currentTime;

        auto genesis = std::make_unique<Block>();
        blockchain.addBlock(std::move(genesis));
        minerGroup.workOn(blockchain);

        do {
            currentTime = minerGroup.nextTimeBlockFound();
            minerGroup.mine(blockchain, currentTime);
            std::cout << currentTime << '\n';

        } while (currentTime < endTime);

        return GameResult();
    }
}
