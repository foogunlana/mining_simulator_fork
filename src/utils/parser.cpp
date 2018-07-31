#include "parser.hpp"

#include "lib/cxxopts/cxxopts.hpp"

#include "utils.hpp"

#include <iostream>

namespace utils {
    Parser::Parser() {}
    
    Args Parser::parse (int argc, char * argv[]) {
        Args args;
        
        try {
            cxxopts::Options options(argv[0], " - command line options");
            options
            .positional_help("[-n --ngames], [-c --commentary]")
            .show_positional_help();
            
            options
            .add_options("commands")
            ("h,help", "Print help")
            ("n,ngames", "number of games in epoch", cxxopts::value<unsigned int>()
                ->default_value("1000"))
            ("x,tx", "expected tx fees gathered in one block", cxxopts::value<unsigned int>()
                ->default_value("50"))
            ("r,reward", "block reward", cxxopts::value<unsigned int>()
                ->default_value("0"))
            ("p,payforward", "initial pay forward to first block", cxxopts::value<unsigned int>()
                ->default_value("0"))
            ("m,miners", "number of miners", cxxopts::value<unsigned int>()
                ->default_value("200"))
            ("d,default-miners", "number of miners stubbornly using honest strategy", cxxopts::value<unsigned int>()
                ->default_value("0"))
            ("s,strategies", "comma [no-space] separated chosen strategies & relative weights", cxxopts::value<std::string>()
                ->default_value("petty:1,payforward:1,lazy:1"))
            ("o,out", "folder name for results", cxxopts::value<std::string>()
                ->default_value("results"))
            ("satoshi-per-bitcoin", "# satoshi in 1 BTC", cxxopts::value<unsigned int>()
                ->default_value("100000000"))
            ("blocks-per-epoch", "expected # blocks in 1 game", cxxopts::value<unsigned int>()
                ->default_value("10000"))
            ("block-time", "expected # of seconds taken to find 1 block", cxxopts::value<unsigned int>()
                ->default_value("600"))
            ("c,commentary", "turn on commentary on games")
            ;
            
            auto result = options.parse(argc, argv);
            if (result.count("help")) {
                std::cout << options.help({"", "commands"}) << std::endl;
                exit(0);
            }
            
            args.numGames = result["n"].as<unsigned int>();
            args.txFees1Block = result["x"].as<unsigned int>();
            args.blockReward = result["r"].as<unsigned int>();
            args.payforward = result["p"].as<unsigned int>();
            args.minerCount = result["m"].as<unsigned int>();
            args.defaultMinerCount = result["d"].as<unsigned int>();
            args.satoshiPerBitcoin = result["satoshi-per-bitcoin"].as<unsigned int>();
            args.expectedNumberOfBlocks = result["blocks-per-epoch"].as<unsigned int>();
            args.expectedTimeToFindBlock = result["block-time"].as<unsigned int>();
            args.out = result["o"].as<std::string>();
            args.commentary = result.count("c") > 0;
            args.strategies = split(result["strategies"].as<std::string>(), ',');
            
        } catch (const cxxopts::OptionException& e) {
            std::cout << "error parsing options: " << e.what() << std::endl;
            exit(1);
        }
        return args;
    }
}
