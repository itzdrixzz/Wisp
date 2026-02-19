#include <iostream>
#include <string>
#include <stdexcept>

struct Options {
    std::string inputPath;
    bool printTokens = false;
};

Options parseArgs(int argc, char** argv);
