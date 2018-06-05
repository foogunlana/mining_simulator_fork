//
//  minerGroup.cpp
//  BlockSim
//
//  Created by Harry Kalodner on 6/9/16.
//  Copyright © 2016 Harry Kalodner. All rights reserved.
//

#include "miner_group.hpp"
#include "miner.hpp"

namespace mining_game {

    MinerGroup::MinerGroup(std::vector<std::unique_ptr<Miner>> miners_) : miners(std::move(miners_)) {}

    BlockTime MinerGroup::nextEventTime(BlockTime time) {
        return BlockTime(1) + time;
    }
}
