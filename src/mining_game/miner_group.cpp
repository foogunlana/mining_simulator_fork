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

#include <limits>
#include <cassert>


// constexpr auto maxTime = BlockTime(std::numeric_limits<BlockTime>::max());


namespace mining_game {

    bool miningSort(const Miner *miner1, const Miner *miner2);
    bool miningSort(const Miner *miner1, const Miner *miner2)  {

        auto miner1Time = miner1->nextMiningTime();
        auto miner2Time = miner2->nextMiningTime();
        return miner1Time > miner2Time;
    }

    MinerGroup::MinerGroup(std::vector<std::unique_ptr<Miner>> miners_) : miners(std::move(miners_)) {
        for (auto &miner : miners) {
            miningQueue.push_back(miner.get());
        }
        std::make_heap(begin(miningQueue), end(miningQueue), miningSort);
    }

    std::unique_ptr<MinerGroup> MinerGroup::build(MinerCount totalMiners) {
        std::vector<std::unique_ptr<Miner>> miners;
        HashRate hashRate(1.0/rawCount(totalMiners));

        for (MinerCount i = 0; i < totalMiners; i++) {
            auto minerName = std::to_string(rawCount(i));
            MinerParameters parameters {rawCount(i), minerName, hashRate};
            miners.push_back(std::make_unique<Miner>(parameters, "strategy2"));
        }
        return std::make_unique<MinerGroup>(std::move(miners));
    }

    void MinerGroup::workOn(Blockchain &chain) {
        for (auto &miner: miners) {
            miner->start(chain);
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
            auto block = miner->mine(chain);
            blocksFound.push_back(std::move(block));
            std::push_heap(begin(miningQueue), end(miningQueue), miningSort);
        }
        for (auto &b : blocksFound) {
            chain.addBlock(std::move(b));
        }
    }

}
