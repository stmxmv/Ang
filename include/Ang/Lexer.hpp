//
// Created by aojoie on 2/22/2023.
//

#ifndef ANG_LEXER_HPP
#define ANG_LEXER_HPP

#include "Token.hpp"

#include <string_view>
#include <unordered_map>

namespace AN {

class KeywordFilter {
    std::unordered_map<std::string_view, tok::TokenKind> HashTable;

    void addKeyword(std::string_view Keyword,
                    tok::TokenKind TokenCode);

public:

    void initializeKeywords();

    tok::TokenKind getKeyword(
            std::string_view Name,
            tok::TokenKind DefaultTokenCode = tok::TokenKind::unknown) {
        auto Result = HashTable.find(Name);

        if (Result != HashTable.end()) {
            return Result->second;
        }
        return DefaultTokenCode;
    }
};

class Lexer {
    const char *source;
    const char *curPtr;
    KeywordFilter keywordFilter;

    void formToken(Token &Result, const char *TokEnd,
                          tok::TokenKind Kind);

    void formIdentifier(Token &result);

    void formNumber(Token &result);

    void formChar(Token &result);

    void formString(Token &result);

    void ignoreSingleLineComment();

    void ignoreMultiLineComment();

public:

    explicit Lexer(const char *aSource) : source(aSource), curPtr(aSource) {
        keywordFilter.initializeKeywords();
    }


    void next(Token &Result);



};

}

#endif//ANG_LEXER_HPP
