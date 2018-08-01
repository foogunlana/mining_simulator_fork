#ifndef configuration_hpp
#define configuration_hpp

#include <vector>
#include <string>

namespace utils {
    
    struct Configuration {
        unsigned int numGames;
        unsigned int txFees1Block;
        unsigned int blockReward;
        unsigned int payforward;
        unsigned int minerCount;
        unsigned int defaultMinerCount;
        unsigned int satoshiPerBitcoin;
        unsigned int expectedNumberOfBlocks;
        unsigned int expectedTimeToFindBlock;
        std::string pathToConfig;
        std::vector<std::pair<std::string, double>> strategies;
        std::string out;
        bool commentary;
        
        Configuration();
        void print(std::ostream& where) const;
    };
    std::ostream& operator<< (std::ostream& out, const Configuration &config);
}

#endif