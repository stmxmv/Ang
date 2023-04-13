//
// Created by aojoie on 4/11/2023.
//

#ifndef ANG_PARSER_HPP
#define ANG_PARSER_HPP

#include <Ang/Lexer.hpp>
#include <Ang/AST.hpp>
#include <unordered_map>
#include <vector>
#include <string>
#include <set>

namespace AN {

typedef std::unordered_map<std::string,
                           std::unordered_map<std::string,
                                              std::vector<std::string>>> LL1Map;

class Parser {

    bool hasError;
    ASTContext &context;
    Lexer &lexer;
    Token token;


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

    Expr *parseExpr();

    Term *parseTerm();

    ExprP *parseExprP();

    TermP *parseTermP();

    Factor *parseFactor();


public:

    explicit Parser(ASTContext &context, Lexer &aLexer) : context(context), lexer(aLexer), hasError() {
        advance();
    }


    AST *parseSimpleExpr();

    std::string parseSimpleExpr(const LL1Map &map,
                                const std::set<std::string> &tSymbols,
                                std::string_view startSymbol);
};



}

#endif//ANG_PARSER_HPP
