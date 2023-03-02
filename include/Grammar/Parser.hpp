//
// Created by aojoie on 3/1/2023.
//

#ifndef ANG_PARSER_HPP
#define ANG_PARSER_HPP

#include "Grammar/Lexer.hpp"
#include "Grammar/Grammar.hpp"
#include <memory>
#include <unordered_map>
#include <unordered_set>

namespace AN::grammar {

class Parser {

    bool hasError;
    ASTContext &context;
    Lexer &lexer;
    Token token;

    std::unordered_map<std::string_view, Symbol *> symbol_map;

    std::unordered_set<Symbol *> left_symbols, right_symbols;

    void advance() {
        lexer.next(token);
    }

    void error();

    bool expect(tok::TokenKind tokenKind);

    bool expectOneOf(tok::TokenKind K1) {
        return expect(K1);
    }

    template<typename... Ts>
    bool expectOneOf(tok::TokenKind K1, Ts... Ks) {
        return expect(K1) || expectOneOf(Ks...);
    }

    bool consume(tok::TokenKind tokenKind) {
        if (!expect(tokenKind)) {
            return false;
        }
        advance();
        return true;
    }

    Symbol *getSymbol(std::string_view val) {
        if (auto iter = symbol_map.find(val); iter != symbol_map.end()) {
            return iter->second;
        }
        return nullptr;
    }

    GrammarTitle *parseGrammarTitle();

    std::vector<Production *> parseProduction();

public:

    explicit Parser(ASTContext &context, Lexer &aLexer) : context(context), lexer(aLexer), hasError() {
        advance();
    }

    Grammar *parse();

};


}

#endif//ANG_PARSER_HPP
