#include "configuration.hpp"

#include <iostream>

namespace utils {
    
    Configuration::Configuration() {}
    
    void Configuration::print(std::ostream& os) const {
        char indent = ' ';
        char newLine = '\n';
        os << "{" << std::endl;
        os << indent << " numGames : " << numGames << ',' << newLine;
        os << indent << " txFees1Block : " << txFees1Block << ',' << newLine;
        os << indent << " blockReward : " << blockReward << ',' << newLine;
        os << indent << " payforward : " << payforward << ',' << newLine;
        os << indent << " minerCount : " << minerCount << ',' << newLine;
        os << indent << " defaultMinerCount : " << defaultMinerCount << ',' << newLine;
        os << indent << " satoshiPerBitcoin : " << satoshiPerBitcoin << ',' << newLine;
        os << indent << " expectedNumberOfBlocks : " << expectedNumberOfBlocks << ',' << newLine;
        os << indent << " expectedTimeToFindBlock : " << expectedTimeToFindBlock << ',' << newLine;
        os << indent << " out : " << out << ',' << newLine;
        os << indent << " commentary : " << (commentary ? "true" : "false") << ',' << newLine;
        os << indent << " strategies : [" << newLine;
        for (auto &s : strategies) {
            os << indent << indent << " - " << s.first << ":" << s.second << ',' << newLine; 
        }
        os << indent << "]" << std::endl;
        os << "}";
    }
    
    std::ostream& operator<< (std::ostream& out, const Configuration& config) {
        config.print(out);
        return out;
    }
}

