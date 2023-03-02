//
// Created by aojoie on 3/1/2023.
//

#ifndef ANG_PARSER_HPP
#define ANG_PARSER_HPP

#include "Grammar/Lexer.hpp"
#include "Grammar/Grammar.hpp"
#include <memory>

namespace AN::grammar {

class Parser {

    bool hasError;
    Lexer &lexer;
    Token token;

    void advance() {
        lexer.next(token);
    }

    void error();

    bool expect(tok::TokenKind tokenKind);

    bool consume(tok::TokenKind tokenKind) {
        if (!expect(tokenKind)) {
            return false;
        }
        advance();
        return true;
    }

public:

    explicit Parser(Lexer &aLexer) : lexer(aLexer), hasError() {
        advance();
    }

    std::shared_ptr<Grammar> parse();

};


}

#endif//ANG_PARSER_HPP
