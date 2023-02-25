//
// Created by aojoie on 2/22/2023.
//

#include "Ang/TokenKinds.hpp"
#include <cstdlib>

static const char * const TokNames[] = {
#define TOK(ID) #ID,
#define KEYWORD(ID, FLAG) #ID,
#include "Ang/TokenKinds.def"
#undef KEYWORD
        nullptr
};

const char *AN::tok::getTokenName(AN::tok::TokenKind Kind) {
    if (Kind < tok::TokenKind::num) {
        return TokNames[(size_t)Kind];
    }
    // TODO error abort
    return nullptr;
}

bool AN::tok::isTokenKeyword(AN::tok::TokenKind kind) {
    switch (kind) {
#define KEYWORD(NAME, FLAGS) \
    case tok::kw_##NAME: return true;
#include "Ang/TokenKinds.def"
#undef KEYWORD
        default:
            break;
    }
    return false;
}

