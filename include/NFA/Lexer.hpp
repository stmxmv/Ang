//
// Created by aojoie on 3/28/2023.
//

#ifndef ANG_LEXER_HPP
#define ANG_LEXER_HPP

#include "NFA/Token.hpp"

#include <unordered_map>
#include <string_view>

namespace AN::NFA {

class KeywordFilter {
    std::unordered_map<std::string_view, tok::TokenKind> HashTable;

    void addKeyword(std::string_view Keyword,
                    tok::TokenKind TokenCode) {
        HashTable.insert({Keyword, TokenCode});
    }

public:
    void initializeKeywords() {
        addKeyword("f", tok::fun_beg);
    }

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

public:
    explicit Lexer(const char *aSource) : source(aSource), curPtr(aSource) {
        keywordFilter.initializeKeywords();
    }


    void next(Token &Result);
};



}
#endif//ANG_LEXER_HPP
