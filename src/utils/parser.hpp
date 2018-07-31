#ifndef parser_hpp
#define parser_hpp

#include <vector>
#include <string>

namespace utils {
    
    struct Args {
        unsigned int numGames;
        unsigned int txFees1Block;
        unsigned int blockReward;
        unsigned int payforward;
        unsigned int minerCount;
        unsigned int defaultMinerCount;
        std::vector<std::string> strategies;
        std::string out;
        bool commentary;
    };
    
    struct Parser {
        Parser();
        Args parse (int argc, char * argv[]);
    };
}

#endif