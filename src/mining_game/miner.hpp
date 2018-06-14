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
#include "block.hpp"

#include <string>
#include <queue>
// #include <memory>

namespace learning_model {
    class Strategy;
}

namespace LM = learning_model;

namespace mining_game {

    class Blockchain;

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
        LM::Strategy &strategy;

        Value totalMiningCost;
        BlockTime _nextMiningTime;
        BlockCount _blocksMinedTotal;
    public:
        const MinerParameters params;
        // Miner(MinerParameters parameters, const Strategy &strategy);
        Miner(MinerParameters parameters, LM::Strategy &strategy, const Blockchain &chain);

        void reset(Blockchain &chain);
        void changeStrategy(LM::Strategy &strategy);
        std::unique_ptr<Block> mine(Blockchain &chain, BlockTime now);

        BlockTime nextMiningTime() const {
            return _nextMiningTime;
        }

        BlockCount getBlocksMinedTotal() const {
            return _blocksMinedTotal;
        }

        void print(std::ostream& where) const;
        friend std::ostream& operator<<(std::ostream& os, const Miner& miner);
    };
}


#endif /* miner_hpp */
