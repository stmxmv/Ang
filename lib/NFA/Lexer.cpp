//
// Created by aojoie on 3/28/2023.
//

#include "NFA/Lexer.hpp"

namespace AN::NFA {

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

    switch (*curPtr) {
        case '(':
            formToken(result, curPtr + 1, tok::left_bracket);
            return;
        case ')':
            formToken(result, curPtr + 1, tok::right_bracket);
            return;
        case '{':
            formToken(result, curPtr + 1, tok::left_curly_bracket);
            return;
        case '}':
            formToken(result, curPtr + 1, tok::right_curly_bracket);
            return;
        case '=':
            formToken(result, curPtr + 1, tok::assign);
            return;
        case ',':
            formToken(result, curPtr + 1, tok::comma);
            return;
        case ';':
            formToken(result, curPtr + 1, tok::semi);
            return;
        default:
            formToken(result, curPtr + 1, tok::unknown);
            return;
    }
}


void Lexer::formIdentifier(Token &result) {
    const char *start = curPtr;
    const char *end   = curPtr + 1;
    while (charinfo::isIdentifierBody(*end)) {
        ++end;
    }

    std::string_view const identifier(start, end - start);

    formToken(result, end, keywordFilter.getKeyword(identifier, tok::identifier));
}

void Lexer::formToken(Token &Result, const char *TokEnd, tok::TokenKind Kind) {
    size_t TokLen = TokEnd - curPtr;
    Result.ptr    = curPtr;

    Result.length = TokLen;
    Result.kind   = Kind;
    curPtr        = TokEnd;
}



}