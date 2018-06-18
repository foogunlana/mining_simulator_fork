
#include "petty_behaviour.hpp"
#include "src/learning_model/behaviour.hpp"

// #include "src/utils/typeDefs.hpp"
#include "src/utils/utils.hpp"

#include "block.hpp"
#include "miner.hpp"
#include "blockchain.hpp"

#include <vector>



namespace mining_game {

    Block & mostRem(const std::vector<std::unique_ptr<Block>> & blocks);

    PettyBehaviour::PettyBehaviour() : learning_model::Behaviour() {}

    Block & PettyBehaviour::chooseParent(const Blockchain & chain, const Miner & miner) const {
        return mostRem(chain.frontier());
    }

    // Block & publish(const Blockchain & chain, const & miner) const {
    //
    // }

    Value PettyBehaviour::collectFees(const Blockchain & chain, const Block & block, const Miner & miner) const {
        return Value(0);
    }

    Value PettyBehaviour::payForward(const Blockchain & chain, const Block & block, Value fees, const Miner & miner) const {
        return Value(0);
    }

    Block & mostRem(const std::vector<std::unique_ptr<Block>> & blocks) {
        Value maxRem = 0;
        for(const auto &block : blocks) {
            maxRem = block->params.rem > maxRem ? block->params.rem : maxRem;
        }
        std::vector<Block *> possiblities;
        for(const auto &block : blocks) {
            if (block->params.rem == maxRem) {
                possiblities.push_back(block.get());
            }
        }
        return *possiblities[utils::selectRandomIndex(possiblities.size())];
    }
}
