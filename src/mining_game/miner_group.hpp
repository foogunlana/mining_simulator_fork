//
//  minerGroup.hpp
//
//

#ifndef minerGroup_hpp
#define minerGroup_hpp

#include "src/utils/typeDefs.hpp"

#include <vector>

namespace mining_game {

    class Blockchain;
    class Miner;

    class MinerGroup {
    public:
        MinerGroup(std::vector<std::unique_ptr<Miner>> miners);
        static std::unique_ptr<MinerGroup> build(MinerCount totalMiners);
        const std::vector<std::unique_ptr<Miner>> miners;
        std::vector<Miner *> miningQueue;

        void reset();
        BlockTime nextTimeBlockFound() const;
        void mine(Blockchain & blockchain, BlockTime untilTime);
        void workOn(Blockchain &chain);
    };
}

#endif /* MinerGroup_hpp */
