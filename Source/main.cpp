#include <bits/stdc++.h>
#include "lexer/lexer.hpp"
#include "arguments/arg.hpp"


static std::string readFileToString(const std::string& path) {
    std::ifstream f(path, std::ios::in | std::ios::binary);
    if (!f) throw std::runtime_error("Error opening file: " + path);
    std::string contents;
    f.seekg(0, std::ios::end);
    contents.resize((size_t)f.tellg());
    f.seekg(0, std::ios::beg);
    f.read(contents.data(), (std::streamsize)contents.size());
    return contents;
}

int main(int argc, char** argv) {
    try
    {
        Options opts = parseArgs(argc, argv);

        std::string src = readFileToString(opts.inputPath);
        Lexer lex(src);

        if (opts.printTokens){
            while(true) {
                Token t = lex.next();
                std::cout << tokenKindName(t.kind)
                    << "  lexeme=`" << t.lexeme << "`"
                    << "  at " << t.span.line_start << ":" << t.span.col_start
                    << " -> " << t.span.line_end << ":" << t.span.col_end
                    << "\n";

                if (t.kind == TokenKind::EndOfFile) break;
            }
            return 0;
        }
    }
    catch(const std::exception& e)
    {
        std::cerr << "Fatal: " << e.what() << "\n";
        return 1;
    }
    return 0;
}