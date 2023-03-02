//
// Created by aojoie on 3/1/2023.
//

#include "Grammar/Parser.hpp"
#include <iostream>
#include <unordered_set>

namespace AN::grammar {


std::shared_ptr<Grammar> Parser::parse() {
    std::string_view grammar_name;
    Symbol startSymbol;

    std::unordered_set<std::string_view> left_symbols, right_symbols;

    /// parse grammar name and start symbol
    if (expect(tok::identifier)) {
        grammar_name = token.getRawData();
        if (consume(tok::left_bracket)) {
            if (expect(tok::identifier)) {
                startSymbol = Symbol(token.getRawData(), Symbol::VSymbol);
                if (!consume(tok::right_bracket) || !consume(tok::delimiter)) {
                    goto __error;
                }
            } else {
                goto __error;
            }
        } else {
            goto __error;
        }
    }

    /// rest
    while (token.isNot(tok::eof)) {
        // parse each statement until ';'
        if (expect(tok::identifier)) {
            left_symbols.insert(token.getRawData());


        } else {
            goto __error;
        }
    }

__error:
    while (token.isNot(tok::eof)) {
        advance();
    }
    return nullptr;
}


void Parser::error() {
    std::cout << "Unexpected" << token.getRawData() << '\n';
    hasError = true;
}

bool Parser::expect(tok::TokenKind tokenKind) {
    if (token.getKind() != tokenKind) {
        error();
        return false;
    }
    return true;
}

}