//
// Created by aojoie on 3/1/2023.
//

#ifndef ANG_LEXER_HPP
#define ANG_LEXER_HPP

#include "Grammar/Token.hpp"

namespace AN::grammar {

class Lexer {
    const char *source;
    const char *curPtr;

    void formToken(Token &Result, const char *TokEnd,
                   tok::TokenKind Kind);

    void formIdentifier(Token &result);

public:
    explicit Lexer(const char *aSource) : source(aSource), curPtr(aSource) {}


    void next(Token &Result);
};


}

#endif//ANG_LEXER_HPP
