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

#include <string>

// #include <queue>
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
        std::string strategy;
    public:
        const MinerParameters params;
        // Miner(MinerParameters parameters, const Strategy &strategy);
        Miner(MinerParameters parameters, const std::string strategy);
    };
}


#endif /* miner_hpp */
