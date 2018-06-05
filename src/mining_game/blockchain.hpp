//
//  blockchain.hpp
//
//

#ifndef blockchain_hpp
#define blockchain_hpp

#include "src/utils/typeDefs.hpp"
#include "blockchain_settings.hpp"

class Block;


namespace mining_game {

    class Blockchain {
    private:
        Value valueNetworkTotal;
        BlockTime timeInSecs;
        BlockRate secondsPerBlock;
        ValueRate transactionFeeRate;

        BlockHeight _maxHeightPub;
    public:
        Blockchain(BlockchainSettings blockchainSettings);
        void advanceToTime(BlockTime time);
        BlockTime getTime() const;
    };
}

#endif /* blockchain_hpp */
