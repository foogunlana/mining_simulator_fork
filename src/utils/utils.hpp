#ifndef utils_hpp
#define utils_hpp

#include "typeDefs.hpp"

#include <cstddef>
#include <vector>
#include <string>

namespace utils {

    std::size_t selectRandomIndex(size_t size);
    double selectRandomChance();
    BlockTime selectMiningOffset(TimeRate mean);

    Value valWithNoise(Value minVal, Value maxVal);
    std::vector<std::string> split(std::string text, char delimiter);
}

#endif
