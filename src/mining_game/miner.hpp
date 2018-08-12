#ifndef miner_hpp
#define miner_hpp

#include "src/utils/typeDefs.hpp"
#include "block.hpp"
#include "src/learning_model/strategy.hpp"

#include <string>
#include <queue>

namespace LM = learning_model;

namespace mining_game {

    class Blockchain;

    struct MinerParameters {
        const unsigned int number;
        const std::string name;
        const HashRate hashRate;
    };

    class Miner {
    private:
        std::reference_wrapper<const LM::Strategy> strategy;
        std::vector<std::unique_ptr<Block>> unbroadcastBlocks;

        Value totalMiningCost;
        BlockTime _nextMiningTime;
        BlockCount _blocksMinedTotal;
    public:
        const MinerParameters params;
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

        std::string getStrategyName() const {
            return strategy.get().name;
        }

        void print(std::ostream& where) const;
        friend std::ostream& operator<<(std::ostream& os, const Miner& miner);
    };
}


#endif
