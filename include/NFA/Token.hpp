//
// Created by aojoie on 3/28/2023.
//

#ifndef ANG_TOKEN_HPP
#define ANG_TOKEN_HPP

#include <string_view>

namespace AN::NFA {

namespace tok {

enum TokenKind {
    unknown,
    eof,
    identifier,
    fun_beg, // f
    left_bracket, // (
    right_bracket, // )
    assign, // =
    comma, //,
    left_curly_bracket, // {
    right_curly_bracket, // }
    semi, // ;
    TOKEN_NUM
};

}

class Token {
    tok::TokenKind kind;
    const char *ptr;
    size_t length;

    friend class Lexer;
public:

    Token() : kind(tok::unknown), ptr(), length() {}

    bool is(tok::TokenKind K) const { return kind == K; }

    bool isNot(tok::TokenKind K) const { return kind != K; }

    bool isOneOf(tok::TokenKind K1, tok::TokenKind K2) const {
        return is(K1) || is(K2);
    }

    template<typename... Ts>
    bool isOneOf(tok::TokenKind K1, tok::TokenKind K2,
                 Ts... Ks) const {
        return is(K1) || isOneOf(K2, Ks...);
    }

    void setKind(tok::TokenKind aKind) {
        Token::kind = aKind;
    }

    tok::TokenKind getKind() const {
        return kind;
    }

    const size_t &getLength() const {
        return length;
    }

    std::string_view getRawData() {
        return {ptr, length};
    }
};

}

#endif//ANG_TOKEN_HPP
