//
// Created by aojoie on 2/21/2023.
//

#ifndef ANG_TOKENKINDS_HPP
#define ANG_TOKENKINDS_HPP

namespace AN::tok {

enum TokenKind : unsigned short {
#define TOK(ID) ID,
#include "TokenKinds.def"
    num
};

enum class TokenCategory : unsigned short {

};

const char *getTokenName(TokenKind Kind);

bool isTokenKeyword(TokenKind kind);

}// namespace AN::tok

#endif//ANG_TOKENKINDS_H
