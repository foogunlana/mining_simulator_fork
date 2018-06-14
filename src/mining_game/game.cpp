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
#include "miner_result.hpp"


#include <iostream>

namespace mining_game {

    Game::Game(GameSettings settings_): blockchainSettings(settings_.blockchainSettings) {}

    GameResult Game::run(MinerGroup &minerGroup, Blockchain &blockchain) {

        BlockTime endTime = blockchainSettings.numberOfBlocks * blockchainSettings.secondsPerBlock;
        BlockTime currentTime;

        auto params = BlockParameters{
            BlockTime(0),
            BlockTime(0),
            Value(0),
            Value(0),
            Value(0),
            Value(0),
        };
        auto genesis = std::make_unique<Block>(params);
        blockchain.addBlock(std::move(genesis));

        do {
            currentTime = minerGroup.nextTimeBlockFound();
            blockchain.advanceToTime(currentTime);
            minerGroup.mine(blockchain, currentTime);
            std::cout << currentTime << '\n';
        } while (currentTime < endTime);

        // Block &winningBlock = blockchain.winningHead();


        return GameResult();
    }
}
