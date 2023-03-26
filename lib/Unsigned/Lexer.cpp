//
// Created by aojoie on 3/21/2023.
//

#include "Unsigned/Lexer.hpp"

namespace AN::Unsigned {
namespace charinfo {

inline bool isASCII(char Ch) {
    return static_cast<unsigned char>(Ch) <= 127;
}

inline bool isVerticalWhitespace(char Ch) {
    return isASCII(Ch) && (Ch == '\r' || Ch == '\n');
}

inline bool isHorizontalWhitespace(char Ch) {
    return isASCII(Ch) && (Ch == ' ' || Ch == '\t' ||
                           Ch == '\f' || Ch == '\v');
}

inline bool isWhitespace(char Ch) {
    return isHorizontalWhitespace(Ch) ||
           isVerticalWhitespace(Ch);
}

inline bool isDigit(char Ch) {
    return isASCII(Ch) && Ch >= '0' && Ch <= '9';
}

inline bool isHexDigit(char Ch) {
    return isASCII(Ch) &&
           (isDigit(Ch) || (Ch >= 'A' && Ch <= 'F'));
}

inline bool isIdentifierHead(char Ch) {
    return isASCII(Ch) &&
           (Ch == '_' || (Ch >= 'A' && Ch <= 'Z') ||
            (Ch >= 'a' && Ch <= 'z'));
}

inline bool isIdentifierBody(char Ch) {
    return isIdentifierHead(Ch) || isDigit(Ch);
}

}// namespace charinfo


void Lexer::next(Token &result) {
    /// skip all kinds of white space
    while (*curPtr && charinfo::isWhitespace(*curPtr)) {
        ++curPtr;
    }

    /// check if end of string
    if (!*curPtr) {
        result.setKind(tok::eof);
        return;
    }

    if (charinfo::isDigit(*curPtr) || *curPtr == '.') {
        formNumber(result);
        return;
    }

    formOther(result);
}

void Lexer::formNumber(Token &result) {
    const char *end   = curPtr;
    int state = 0;

    for (;;) {
        switch (state) {
            case 0:
                if (charinfo::isDigit(*end)) {
                    state = 1;
                } else if (*end == '.') {
                    state = 3;
                } else {
                    goto done;
                }
                break;
            case 1:
                if (charinfo::isDigit(*end)) {
                    (void)0;
                } else if (*end == '.') {
                    state = 2;
                } else if (*end == 'E') {
                    state = 4;
                } else {
                    goto done;
                }
                break;
            case 2:
                if (charinfo::isDigit(*end)) {
                    (void)0;
                } else if (*end == 'E') {
                    state = 4;
                } else {
                    goto done;
                }
                break;
            case 3:
                if (charinfo::isDigit(*end)) {
                    state = 2;
                } else {
                    goto done;
                }
                break;
            case 4:
                if (charinfo::isDigit(*end)) {
                    state = 6;
                } else if (*end == '+' || *end == '-') {
                    state = 5;
                } else {
                    goto done;
                }
                break;
            case 5:
                if (charinfo::isDigit(*end)) {
                    state = 6;
                } else {
                    goto done;
                }
                break;
            case 6:
                if (charinfo::isDigit(*end)) {
                    (void)0;
                } else {
                    goto done;
                }
                break;
            default:
                goto error;
        }
        ++end;
    }

done:
    if (state == 1 || state == 2 || state == 6) {
        formToken(result, end, tok::number);
        return;
    }

error:
    formToken(result, end, tok::error);
}

void Lexer::formOther(Token &result) {
    const char *end = curPtr + 1;
    while (*end && !charinfo::isDigit(*end) && *end != '.' && !charinfo::isWhitespace(*end)) {
        ++end;
    }
    formToken(result, end, tok::other);
}

void Lexer::formToken(Token &Result, const char *TokEnd, tok::TokenKind Kind) {
    size_t TokLen = TokEnd - curPtr;
    Result.ptr    = curPtr;

    Result.length = TokLen;
    Result.kind   = Kind;
    curPtr        = TokEnd;
}

}