#include "arg.hpp"

static void printHelp(const char* prog) {
    std::cout
        << "Usage:\n"
        << "  " << prog << " <input.wisp> [options]\n"
        << "Options:\n"
        << "  --tokens         Print lexer tokens and exit\n"
        << "  -h, --help       Show this help\n";
}


Options parseArgs(int argc, char** argv) {
    Options opts;

    for (int i = 1; i < argc; i++) {
        std::string arg = argv[i];

        if (arg == "-h" || arg == "--help") {
            printHelp(argv[0]);
            std::exit(0);
        }
        else if (arg == "--tokens") {
            opts.printTokens = true;
        }
        else if (!arg.empty() && arg[0] == '-') {
            throw std::runtime_error("Unknown option: " + arg);
        }
        else {
            if (!opts.inputPath.empty()) {
                throw std::runtime_error(
                    "Multiple input files given: '" +
                    opts.inputPath + "' and '" + arg + "'"
                );
            }
            opts.inputPath = arg;
        }
    }

    if (opts.inputPath.empty()) {
        throw std::runtime_error("No input file provided");
    }

    return opts;
}
