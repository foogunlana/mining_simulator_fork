#ifndef minerGroup_hpp
#define minerGroup_hpp

#include "src/utils/typeDefs.hpp"

#include <vector>

namespace learning_model {
    class Strategy;
    struct PlayerProfile;
}

namespace LM = learning_model;

namespace mining_game {

    class Blockchain;
    class Miner;

    class MinerGroup {
    private:
        std::vector<LM::Strategy *> learningStrategies;

    public:
        MinerGroup(
            std::vector<std::unique_ptr<Miner>> miners,
            std::vector<Miner *> learningMiners,
            std::vector<LM::Strategy *> learningStrategies
        );
        static std::unique_ptr<MinerGroup> build(
            MinerCount totalMiners,
            MinerCount numDefault,
            std::vector<LM::Strategy *> learningStrategies,
            LM::Strategy *defaultStrategy,
            const Blockchain &chain
        );

        const std::vector<std::unique_ptr<Miner>> miners;
        const std::vector<Miner *> learningMiners;
        std::vector<Miner *> miningQueue;

        BlockTime nextTimeBlockFound() const;
        const std::vector<Miner *> & getLearningMiners() const;
        void reset(Blockchain &chain);
        void mine(Blockchain & blockchain, BlockTime untilTime);
        void updateLearningMinerStrategies(const std::vector<LM::PlayerProfile> & strategyProfiles);
    };
}

#endif
