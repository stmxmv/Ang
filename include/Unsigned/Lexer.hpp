//
// Created by aojoie on 3/21/2023.
//

#ifndef ANG_LEXER_HPP
#define ANG_LEXER_HPP

#include "Unsigned/Token.hpp"


namespace AN::Unsigned {


class Lexer {
    const char *curPtr;

    void formOther(Token &result);

    void formNumber(Token &result);

    void formToken(Token &Result, const char *TokEnd,
                   tok::TokenKind Kind);

public:

    explicit Lexer(const char *aSource) : curPtr(aSource) {}


    void next(Token &result);

};

}


#endif//ANG_LEXER_HPP
