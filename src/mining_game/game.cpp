//
//  game.cpp
//  BlockSim
//
//  Created by Harry Kalodner on 6/6/16.
//  Copyright © 2016 Harry Kalodner. All rights reserved.
//

#include "miner_group.hpp"
#include "game.hpp"
#include "game_result.hpp"


namespace mining_game {

    Game::Game(GameSettings settings_): settings(settings_) {}

    GameResult Game::run(MinerGroup &minerGroup) {
        return GameResult();
    }
}
