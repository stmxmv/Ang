//
// Created by aojoie on 2/22/2023.
//

#ifndef ANG_TOKEN_HPP
#define ANG_TOKEN_HPP

#include "TokenKinds.hpp"
#include <cassert>
#include <cstddef>
#include <string_view>

namespace AN {

class Token {

    tok::TokenKind kind;
    const char *ptr;
    size_t length;

    friend class Lexer;

public:

    Token() : kind(tok::TokenKind::unknown), ptr(), length() {}

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

    std::string_view getIdentifier() {
        assert(is(tok::TokenKind::identifier) && "Cannot get identfier of non-identifier");
        return { ptr, length };
    }

    const char *getName() const {
        return tok::getTokenName(kind);
    }

    std::string_view getRawData() {
        return { ptr, length };
    }

    bool isKeyword() const {
        return tok::isTokenKeyword(kind);
    }

};


}

#endif//ANG_TOKEN_HPP
