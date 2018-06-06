//
//  miner.hpp
//  BlockSim
//
//  Created by Harry Kalodner on 5/25/16.
//  Copyright © 2016 Harry Kalodner. All rights reserved.
//

#ifndef miner_hpp
#define miner_hpp

#include "src/utils/typeDefs.hpp"
#include "blockchain.hpp"
#include "block.hpp"

#include <string>
#include <queue>
// #include <memory>

namespace mining_game {

    struct MinerParameters {
        const unsigned int number;
        const std::string name;
        const HashRate hashRate;
        // const BlockTime networkDelay;
        // const ValueRate costPerSecond;
    };

    class Miner {
    private:
        // std::reference_wrapper<const Strategy> strategy;
        std::vector<std::unique_ptr<Block>> unbroadcastBlocks;
        std::string strategy;
        Blockchain *blockchain;

        Value totalMiningCost;
        BlockTime _nextMiningTime;
    public:
        const MinerParameters params;
        // Miner(MinerParameters parameters, const Strategy &strategy);
        Miner(MinerParameters parameters, const std::string strategy);

        void workOn(Blockchain &blockchain);
        BlockTime nextMiningTime() const;
        Block mine();
    };
}


#endif /* miner_hpp */