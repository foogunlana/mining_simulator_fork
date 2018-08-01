#ifndef configuration_helper_hpp
#define configuration_helper_hpp


#include <string>

namespace utils {
    
    struct Configuration;

    struct ConfigurationHelper {
        ConfigurationHelper();
        Configuration parse (int argc, char * argv[], std::string defaultConfigPath);
        Configuration configFromFile(std::string pathToConfig) const;
    };
}

#endif