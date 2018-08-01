#include "configuration_helper.hpp"
#include "configuration.hpp"

#include "lib/cxxopts/cxxopts.hpp"

#include "utils.hpp"
#include "lib/json/json.hpp"

#include <iostream>
#include <fstream>
#include <map>

namespace utils {
    ConfigurationHelper::ConfigurationHelper() {}
        
    Configuration ConfigurationHelper::parse (int argc, char * argv[], std::string defaultConfigPath) {
        Configuration config;
        try {
            cxxopts::Options options(argv[0], " - command line options");
            options
            .positional_help("[-n --ngames], [-c --commentary]")
            .show_positional_help();
            
            options
            .add_options("commands")
            ("h,help", "Print help message")
            ("n,ngames", "number of games in epoch", cxxopts::value<unsigned int>())
            ("x,tx", "expected tx fees gathered in one block", cxxopts::value<unsigned int>())
            ("r,reward", "block reward", cxxopts::value<unsigned int>())
            ("p,payforward", "initial pay forward to first block", cxxopts::value<unsigned int>())
            ("m,miners", "number of miners", cxxopts::value<unsigned int>())
            ("d,default-miners", "number of miners stubbornly using honest strategy", cxxopts::value<unsigned int>())
            ("s,strategies", "comma [no-space] separated chosen strategies & relative weights", cxxopts::value<std::string>())
            ("o,out", "folder name for results", cxxopts::value<std::string>())
            ("satoshi-per-bitcoin", "# satoshi in 1 BTC", cxxopts::value<unsigned int>())
            ("blocks-per-epoch", "expected # blocks in 1 game", cxxopts::value<unsigned int>())
            ("block-time", "expected # of seconds taken to find 1 block", cxxopts::value<unsigned int>())
            ("config", "path to the config.json file containing parameters", cxxopts::value<std::string>())
            ("y,modify-strategies", "use strategies in config but replace appropriate ones with args provided", cxxopts::value<std::string>())
            ("c,commentary", "turn on commentary on games")
            ;
            
            auto result = options.parse(argc, argv);
            std::string pathToConfig = defaultConfigPath;
            if (result.count("config")) {
                pathToConfig = result["config"].as<std::string>();
            }
            config = configFromFile(pathToConfig);
            config.commentary = result.count("c") > 0;
            if (result.count("help")) {
                std::cout << options.help({"", "commands"}) << std::endl;
                exit(0);
            }
            if (result.count("n")) {
                config.numGames = result["n"].as<unsigned int>();
            }
            if (result.count("x")) {
                config.txFees1Block = result["x"].as<unsigned int>();
            }
            if (result.count("r")) {
                config.blockReward = result["r"].as<unsigned int>();
            }
            if (result.count("p")) {
                config.payforward = result["p"].as<unsigned int>();
            }
            if (result.count("m")) {
                config.minerCount = result["m"].as<unsigned int>();
            }
            if (result.count("d")) {
                config.defaultMinerCount = result["d"].as<unsigned int>();
            }
            if (result.count("satoshi-per-bitcoin")) {
                config.satoshiPerBitcoin = result["satoshi-per-bitcoin"].as<unsigned int>();
            }
            if (result.count("blocks-per-epoch")) {
                config.expectedNumberOfBlocks = result["blocks-per-epoch"].as<unsigned int>();
            }
            if (result.count("block-time")) {
                config.expectedTimeToFindBlock = result["block-time"].as<unsigned int>();
            }
            if (result.count("o")) {
                config.out = result["o"].as<std::string>();
            }
            if (result.count("strategies") && (result.count("modify-strategies"))) {
                std::cout << "Cannot specify strategies and strategy modifiers together" << std::endl;
                exit(1);
            }
            if (result.count("strategies")) {
                config.strategies.clear();
                std::vector<std::string> strategies = split(result["strategies"].as<std::string>(), ',');
                for (auto &s : strategies) {
                    std::vector<std::string> nw = split(s, ':');
                    std::string name = nw[0];
                    double weight = nw.size() == 2 ? stod(nw[1]) : 1;
                    config.strategies.push_back(std::make_pair(name, weight));
                }
            } else if (result.count("modify-strategies")) {
                std::vector<std::string> strategies = split(result["modify-strategies"].as<std::string>(), ',');
                std::map<std::string, size_t> indices;
                for (size_t i = 0; i < config.strategies.size(); i++) {
                    indices[config.strategies[i].first] = i;
                }
                for (auto &s : strategies) {
                    std::vector<std::string> nw = split(s, ':');
                    std::string name = nw[0];
                    if (indices.find(name) == indices.end()) {
                        std::cout << "No such strategy '" << name << "' to modify";
                        exit(1);
                    }
                    double weight = nw.size() == 2 ? stod(nw[1]) : 1;
                    config.strategies[indices[name]] = std::make_pair(name, weight);
                }
            }
        } catch (const cxxopts::OptionException& e) {
            std::cout << "error parsing options: " << e.what() << std::endl;
            exit(1);
        }
        return config;
    }
    
    Configuration ConfigurationHelper::configFromFile(std::string pathToConfig) const {
        nlohmann::json configJson;
        std::ifstream(pathToConfig) >> configJson;
        Configuration config;
        config.numGames = configJson["numGames"].get<unsigned int>();
        config.txFees1Block = configJson["txFees1Block"].get<unsigned int>();
        config.blockReward = configJson["blockReward"].get<unsigned int>();
        config.payforward = configJson["payforward"].get<unsigned int>();
        config.minerCount = configJson["minerCount"].get<unsigned int>();
        config.defaultMinerCount = configJson["defaultMinerCount"].get<unsigned int>();
        config.satoshiPerBitcoin = configJson["satoshiPerBitcoin"].get<unsigned int>();
        config.expectedNumberOfBlocks = configJson["expectedNumberOfBlocks"].get<unsigned int>();
        config.expectedTimeToFindBlock = configJson["expectedTimeToFindBlock"].get<unsigned int>();
        config.out = configJson["out"].get<std::string>();
        config.commentary = configJson["commentary"].get<bool>();
        for (auto &s : configJson["strategies"]) {
            std::string name = s[0];
            double weight = s[1];
            config.strategies.push_back(std::make_pair(name, weight));
        }
        return config;
    }
}
