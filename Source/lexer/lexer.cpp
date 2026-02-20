#include "lexer.hpp"
#include <unordered_map>
#include <cctype>
#include <utility>

using namespace std;

static bool isAlpha(char c) { return std::isalpha((unsigned char)c) != 0; }
static bool isDigit(char c) { return std::isdigit((unsigned char)c) != 0; }
static bool isAlphaNum(char c) { return isAlpha(c) || isDigit(c) || c == '_'; }

static bool isAtEnd(const Lexer& L) { return L.pos >= L.source.size(); }

static char peek(const Lexer& L) {
    return isAtEnd(L) ? '\0' : L.source[L.pos];
}

static char peekNext(const Lexer& L) {
    if (L.pos + 1 >= L.source.size()) return '\0';
    return L.source[L.pos + 1];
}

static char advance(Lexer& L) {
    char c = L.source[L.pos++];
    if (c == '\n') { L.line++; L.col = 1; }
    else { L.col++; }
    return c;
}

static Token makeToken(TokenKind kind, string lex, int sl, int sc, int el, int ec) {
    return Token{kind, move(lex), Span{sl, sc, el, ec}};
}

static void skipWhitespaceAndComments(Lexer& L) {
    while (!isAtEnd(L)) {
        char c = peek(L);

        // whitespace (including newlines)
        if (c == ' ' || c == '\t' || c == '\r' || c == '\n') {
            advance(L);
            continue;
        }

        // // comments
        if (c == '/' && peekNext(L) == '/') {
            advance(L); advance(L); // eat //
            while (!isAtEnd(L) && peek(L) != '\n') advance(L);
            continue;
        }

        break;
    }
}

static Token lexIdentifierOrKeyword(Lexer& L, char first, int startLine, int startCol) {
    string text;
    text.push_back(first);

    while (!isAtEnd(L) && isAlphaNum(peek(L))) {
        text.push_back(advance(L));
    }

    static const unordered_map<string, TokenKind> keywords = {
        {"import", TokenKind::KwImport},
        {"type", TokenKind::KwType},
        {"function", TokenKind::KwFunction},
        {"if", TokenKind::KwIf},
        {"else", TokenKind::KwElse},
        {"return", TokenKind::KwReturn},
        {"app", TokenKind::KwApp},
        {"const", TokenKind::KwConst},
        {"main", TokenKind::KwMain},
    };

    auto it = keywords.find(text);
    TokenKind kind = (it != keywords.end()) ? it->second : TokenKind::Identifier;
    return makeToken(kind, text, startLine, startCol, L.line, L.col);
}

static Token lexNumberOrVersion(Lexer& L, char first, int startLine, int startCol) {
    string text;
    text.push_back(first);

    int dotCount = 0;

    while (!isAtEnd(L)) {
        char c = peek(L);

        if (isDigit(c)) {
            text.push_back(advance(L));
            continue;
        }

        if (c == '.' && isDigit(peekNext(L))) {
            dotCount++;
            text.push_back(advance(L)); // '.'
            continue;
        }

        break;
    }

    TokenKind kind = (dotCount >= 2) ? TokenKind::Version : TokenKind::Number;
    return makeToken(kind, text, startLine, startCol, L.line, L.col);
}

static Token lexString(Lexer& L, int startLine, int startCol) {
    string value;

    while (!isAtEnd(L)) {
        char c = advance(L);

        if (c == '"') {
            return makeToken(TokenKind::String, value, startLine, startCol, L.line, L.col);
        }

        if (c == '\n') {
            return makeToken(TokenKind::Invalid, "Unterminated string literal", startLine, startCol, L.line, L.col);
        }

        if (c == '\\') {
            if (isAtEnd(L)) break;
            char e = advance(L);
            switch (e) {
                case 'n': value.push_back('\n'); break;
                case 't': value.push_back('\t'); break;
                case 'r': value.push_back('\r'); break;
                case '"': value.push_back('"'); break;
                case '\\': value.push_back('\\'); break;
                default: value.push_back(e); break;
            }
        } else {
            value.push_back(c);
        }
    }

    return makeToken(TokenKind::Invalid, "Unterminated string literal", startLine, startCol, L.line, L.col);
}

const char* tokenKindName(TokenKind k) {
    switch (k) {
        case TokenKind::EndOfFile: return "EOF";
        case TokenKind::Invalid: return "INVALID";

        case TokenKind::KwImport: return "KW_IMPORT";
        case TokenKind::KwType: return "KW_TYPE";
        case TokenKind::KwFunction: return "KW_FUNCTION";
        case TokenKind::KwIf: return "KW_IF";
        case TokenKind::KwElse: return "KW_ELSE";
        case TokenKind::KwReturn: return "KW_RETURN";
        case TokenKind::KwApp: return "KW_APP";
        case TokenKind::KwConst: return "KW_CONST";
        case TokenKind::KwMain: return "KW_MAIN";

        case TokenKind::Identifier: return "IDENT";
        case TokenKind::Number: return "NUMBER";
        case TokenKind::Version: return "VERSION";
        case TokenKind::String: return "STRING";

        case TokenKind::At: return "AT";
        case TokenKind::LParen: return "LPAREN";
        case TokenKind::RParen: return "RPAREN";
        case TokenKind::LBrace: return "LBRACE";
        case TokenKind::RBrace: return "RBRACE";
        case TokenKind::Comma: return "COMMA";
        case TokenKind::Dot: return "DOT";
        case TokenKind::Colon: return "COLON";
        case TokenKind::Semicolon: return "SEMICOLON";

        case TokenKind::Plus: return "PLUS";
        case TokenKind::Minus: return "MINUS";
        case TokenKind::Star: return "STAR";
        case TokenKind::Slash: return "SLASH";
        case TokenKind::Equal: return "EQUAL";
        case TokenKind::Less: return "LESS";
        case TokenKind::Greater: return "GREATER";

        case TokenKind::EqualEqual: return "EQUALEQUAL";
        case TokenKind::LessEqual: return "LESSEQUAL";
        case TokenKind::GreaterEqual: return "GREATEREQUAL";
        case TokenKind::NotEqual: return "NOTEQUAL";

    }
    return "???";
}

Lexer::Lexer(std::string src)
    : source(std::move(src)), pos(0), line(1), col(1) {}

Token Lexer::next() {
    skipWhitespaceAndComments(*this);

    int startLine = line;
    int startCol  = col;

    if (isAtEnd(*this)) {
        return makeToken(TokenKind::EndOfFile, "", startLine, startCol, line, col);
    }

    char c = advance(*this);

    if (isAlpha(c) || c == '_') {
        return lexIdentifierOrKeyword(*this, c, startLine, startCol);
    }

    if (isDigit(c)) {
        return lexNumberOrVersion(*this, c, startLine, startCol);
    }

    if (c == '"') {
        return lexString(*this, startLine, startCol);
    }

    if (c == '=' && peek(*this) == '='){
        advance(*this);
        return makeToken(TokenKind::EqualEqual, "==", startLine, startCol, line, col);
    }

    if (c == '<' && peek(*this) == '=') {
        advance(*this);
        return makeToken(TokenKind::LessEqual, "<=", startLine, startCol, line, col);
    }

    if (c == '>' && peek(*this) == '=') {
        advance(*this);
        return makeToken(TokenKind::GreaterEqual, ">=", startLine, startCol, line, col);
    }

    if (c == '!' && peek(*this) == '=') {
        advance(*this);
        return makeToken(TokenKind::NotEqual, "!=", startLine, startCol, line, col);
    }



    switch (c) {
        case '@': return makeToken(TokenKind::At, "@", startLine, startCol, line, col);

        case '(': return makeToken(TokenKind::LParen, "(", startLine, startCol, line, col);
        case ')': return makeToken(TokenKind::RParen, ")", startLine, startCol, line, col);
        case '{': return makeToken(TokenKind::LBrace, "{", startLine, startCol, line, col);
        case '}': return makeToken(TokenKind::RBrace, "}", startLine, startCol, line, col);

        case ',': return makeToken(TokenKind::Comma, ",", startLine, startCol, line, col);
        case '.': return makeToken(TokenKind::Dot, ".", startLine, startCol, line, col);
        case ':': return makeToken(TokenKind::Colon, ":", startLine, startCol, line, col);
        case ';': return makeToken(TokenKind::Semicolon, ";", startLine, startCol, line, col);

        case '+': return makeToken(TokenKind::Plus, "+", startLine, startCol, line, col);
        case '-': return makeToken(TokenKind::Minus, "-", startLine, startCol, line, col);
        case '*': return makeToken(TokenKind::Star, "*", startLine, startCol, line, col);
        case '/': return makeToken(TokenKind::Slash, "/", startLine, startCol, line, col);
        case '=': return makeToken(TokenKind::Equal, "=", startLine, startCol, line, col);
        case '<': return makeToken(TokenKind::Less, "<", startLine, startCol, line, col);
        case '>': return makeToken(TokenKind::Greater, ">", startLine, startCol, line, col);

    }

    return makeToken(TokenKind::Invalid, string("Unexpected character: ") + c, startLine, startCol, line, col);
}
