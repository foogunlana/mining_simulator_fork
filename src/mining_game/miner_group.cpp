//
//  minerGroup.cpp
//  BlockSim
//
//  Created by Harry Kalodner on 6/9/16.
//  Copyright © 2016 Harry Kalodner. All rights reserved.
//

#include "miner_group.hpp"
#include "miner.hpp"
#include "block.hpp"
#include "blockchain.hpp"
#include "src/utils/typeDefs.hpp"
#include "src/learning_model/strategy.hpp"
#include "src/learning_model/player_profile.hpp"

#include <limits>
#include <cassert>
#include <iostream>

// constexpr auto maxTime = BlockTime(std::numeric_limits<BlockTime>::max());

namespace LM = learning_model;

namespace mining_game {

    bool miningSort(const Miner *miner1, const Miner *miner2);
    bool miningSort(const Miner *miner1, const Miner *miner2)  {

        auto miner1Time = miner1->nextMiningTime();
        auto miner2Time = miner2->nextMiningTime();
        return miner1Time > miner2Time;
    }

    MinerGroup::MinerGroup(
        std::vector<std::unique_ptr<Miner>> miners_,
        std::vector<Miner *> learningMiners_,
        std::vector<LM::Strategy *> learningStrategies_
    ) :  learningStrategies(learningStrategies_), miners(std::move(miners_)), learningMiners(learningMiners_) {

        for (auto &miner : miners) {
            miningQueue.push_back(miner.get());
        }
        std::make_heap(begin(miningQueue), end(miningQueue), miningSort);
    }

    void MinerGroup::reset(Blockchain &chain) {
        for (auto &miner : miners) {
            miner->reset(chain);
        }
        std::make_heap(begin(miningQueue), end(miningQueue), miningSort);
    }

    std::unique_ptr<MinerGroup> MinerGroup::build(
        MinerCount totalMiners,
        MinerCount numDefault,
        std::vector<LM::Strategy *> learningStrategies,
        LM::Strategy *defaultStrategy,
        const Blockchain &chain
    ) {
        std::vector<std::unique_ptr<Miner>> miners;
        std::vector<Miner *> learningMiners;

        HashRate hashRate(1.0/rawCount(totalMiners));
        MinerCount numberRandomMiners(totalMiners - numDefault);

        for (MinerCount i = 0; i < totalMiners; i++) {
            auto minerName = std::to_string(rawCount(i));
            MinerParameters parameters {rawCount(i), minerName, hashRate};
            miners.push_back(std::make_unique<Miner>(parameters, *defaultStrategy, chain));
            if (i < numberRandomMiners) {
                learningMiners.push_back(miners.back().get());
            }
        }

        return std::make_unique<MinerGroup>(
            std::move(miners), learningMiners, learningStrategies);
    }

    void MinerGroup::updateLearningMinerStrategies(const std::vector<LM::PlayerProfile> & profiles) {
        assert(learningMiners.size() == profiles.size());

        for (size_t miner = 0; miner < learningMiners.size(); miner++) {
            LM::Strategy *strategy = learningStrategies[profiles[miner].currentStrategy];
            learningMiners[miner]->changeStrategy(*strategy);
        }
    }

    BlockTime MinerGroup::nextTimeBlockFound() const {
        return miningQueue.front()->nextMiningTime();
    }

    void MinerGroup::mine(Blockchain & chain, BlockTime now) {
        std::vector<std::unique_ptr<Block>> blocksFound;
        while (miningQueue.front()->nextMiningTime() == now) {
            std::pop_heap(begin(miningQueue), end(miningQueue), miningSort);
            Miner *miner = miningQueue.back();
            auto block = miner->mine(chain, now);
            blocksFound.push_back(std::move(block));
            std::push_heap(begin(miningQueue), end(miningQueue), miningSort);
        }
        for (auto &b : blocksFound) {
            chain.addBlock(std::move(b));
        }
    }

}
