//
// Created by aojoie on 3/1/2023.
//

#include "Grammar/Lexer.hpp"

namespace AN::grammar {

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

    if (charinfo::isIdentifierHead(*curPtr)) {
        formIdentifier(result);
        return;
    }

    if (charinfo::isDigit(*curPtr)) {
        formNumber(result);
        return;
    }
    if (*curPtr == '\'') {
        formChar(result);
        return;
    }
    if (*curPtr == '"') {
        formString(result);
        return;
    }

    switch (*curPtr) {
        case '[':
            formToken(result, curPtr + 1, tok::left_bracket);
            return;
        case ']':
            formToken(result, curPtr + 1, tok::right_bracket);
            return;
        case ';':
            formToken(result, curPtr + 1, tok::delimiter);
            return;
        case '|':
            formToken(result, curPtr + 1, tok::alter);
            return;
        case '-':
            if (*(curPtr + 1) == '>') {
                formToken(result, curPtr + 2, tok::separator);
            } else {
                formToken(result, curPtr + 1, tok::unknown);
            }
            return;
        default:
            formToken(result, curPtr + 1, tok::unknown);
            return;
    }
}

void Lexer::formNumber(Token &result) {
    const char *start = curPtr;
    const char *end   = curPtr + 1;

    bool point = false;

    // check hex start
    if (*start == '0' && *end == 'x') {
        ++end;
    }

    for (;;) {

        if (*end == '.') {
            if (!point) {
                point = true;
            } else {
                /// TODO illegal number report, double point
            }

        } else if (!charinfo::isDigit(*end)) {
            break;
        }

        ++end;
    }

    /// TODO check number literal suffix

    if (point) {
        formToken(result, end, tok::float_literal);
    } else {
        formToken(result, end, tok::integer_literal);
    }
}

void Lexer::formChar(Token &result) {
    const char *start = curPtr;
    const char *end   = curPtr + 1;

    while (*end && (*end != *start || *(end - 1) == '\\') && !charinfo::isVerticalWhitespace(*end)) {
        ++end;
    }

    /// TODO report error on *end is vertical whitespace

    /// *end = *start
    formToken(result, end + 1, tok::char_literal);
}

void Lexer::formString(Token &result) {
    const char *start = curPtr;
    const char *end   = curPtr + 1;

    while (*end && (*end != *start || *(end - 1) == '\\') && !charinfo::isVerticalWhitespace(*end)) {
        ++end;
    }
    /// TODO report error on *end is vertical whitespace

    /// *end = *start
    formToken(result, end + 1, tok::string_literal);
}

void Lexer::formIdentifier(Token &result) {
    const char *start = curPtr;
    const char *end   = curPtr + 1;
    while (charinfo::isIdentifierBody(*end)) {
        ++end;
    }

    formToken(result, end, tok::identifier);
}

void Lexer::formToken(AN::grammar::Token &Result, const char *TokEnd, AN::grammar::tok::TokenKind Kind) {
    size_t TokLen = TokEnd - curPtr;
    Result.ptr    = curPtr;

    Result.length = TokLen;
    Result.kind   = Kind;
    curPtr        = TokEnd;
}


}