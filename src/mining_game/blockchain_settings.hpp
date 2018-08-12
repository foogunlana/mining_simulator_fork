#ifndef blockchain_settings_hpp
#define blockchain_settings_hpp

#include "src/utils/typeDefs.hpp"

namespace mining_game {

    struct BlockchainSettings {
        BlockRate secondsPerBlock;
        ValueRate transactionFeeRate;
        BlockValue blockReward;
        BlockCount numberOfBlocks;
        Value payforward;
    };
}

#endif
