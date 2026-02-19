#pragma once
#include <string>

enum class TokenKind {
    EndOfFile,
    Invalid,
    KwImport,
    KwType,
    KwFunction,
    KwReturn,
    KwApp,
    KwConst,
    KwMain,
    Identifier,
    Number,
    Version,
    String,
    At,
    LParen, RParen,
    LBrace, RBrace,
    Comma, Dot, Colon, Semicolon,
    Plus, Star, Equal, Slash, Minus,
};

const char* tokenKindName(TokenKind k);

struct Span {
    int line_start, col_start;
    int line_end, col_end;
};

struct Token {
    TokenKind kind;
    std::string lexeme;
    Span span;
};

struct Lexer {
    std::string source;
    size_t pos;
    int line;
    int col;

    Lexer(std::string src);

    Token next();
};