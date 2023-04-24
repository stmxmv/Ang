//
// Created by aojoie on 3/1/2023.
//

#ifndef ANG_PARSER_HPP
#define ANG_PARSER_HPP

#include "Grammar/Lexer.hpp"
#include "Grammar/Grammar.hpp"
#include "Grammar/AST.hpp"
#include <memory>
#include <unordered_map>
#include <unordered_set>

namespace AN::grammar {

class Parser {

    bool hasError;
    ASTContext &context;
    Lexer &lexer;
    Token token;

    int productionID{};

    std::unordered_map<std::string_view, Symbol *> symbol_map;

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
        if (token.isOneOf(K1, Ks...)) {
            return true;
        }
        error();
        return false;
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

    AST *parseAST();
};


}

#endif//ANG_PARSER_HPP
