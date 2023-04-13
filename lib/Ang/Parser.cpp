//
// Created by aojoie on 4/11/2023.
//

#include "Ang/Parser.hpp"
#include <iostream>
#include <stack>

namespace AN {


void Parser::error() {
    std::cout << "Unexpected " << token.getRawData() << '\n';
    hasError = true;
}

bool Parser::expect(tok::TokenKind tokenKind) {
    if (token.getKind() != tokenKind) {
        error();
        return false;
    }
    return true;
}

Expr *Parser::parseExpr() {
    Term *term = parseTerm();
    ExprP *exprP = parseExprP();
    if (term && exprP) {
        return new (context) Expr(term, exprP);
    }
    return nullptr;
}


Term *Parser::parseTerm() {
    Factor *factor = parseFactor();
    TermP *termP = parseTermP();
    if (factor && termP) {
        return new (context) Term(factor, termP);
    }
    return nullptr;
}


ExprP *Parser::parseExprP() {
    AddOp *addOp;
    Term *term;
    ExprP *exprP;
    if (token.isOneOf(tok::plus, tok::minus)) {

        addOp = new (context) AddOp(token.getRawData());

        advance();

    } else {
        /// return empty ExprP
        return new (context) ExprP();
    }

    term = parseTerm();
    exprP = parseExprP();

    if (term && exprP) {
        return new (context) ExprP(addOp, term, exprP);
    }

    return nullptr;
}

TermP *Parser::parseTermP() {
    MulOp *mulOp;
    Factor *factor;
    TermP *termP;

    if (token.isOneOf(tok::star, tok::division)) {

        mulOp = new (context) MulOp(token.getRawData());

        advance();

    } else {
        /// return empty TermP
        return new (context) TermP();
    }

    factor = parseFactor();
    termP = parseTermP();

    if (factor && termP) {
        return new (context) TermP(mulOp, factor, termP);
    }
    return nullptr;
}

Factor *Parser::parseFactor() {
    if (token.isOneOf(tok::identifier, tok::integer_literal, tok::float_literal)) {

        ID *id = new (context) ID(token.getRawData());
        advance();
        return new (context) Factor(id);
    }

    if (expect(tok::left_bracket)) {
        advance();
        Expr *expr = parseExpr();
        if (expr) {
            if (consume(tok::right_bracket)) {
                BracketExpr *bracketExpr = new (context) BracketExpr(expr);
                return new (context) Factor(bracketExpr);
            }
        }

    }
    return nullptr;
}

AST *Parser::parseSimpleExpr() {
    return parseExpr();
}

std::string Parser::parseSimpleExpr(const LL1Map &map,
                                    const std::set<std::string> &tSymbols,
                                    std::string_view startSymbol) {
    std::string result;

    std::stack<std::string> stk;
    stk.emplace("#");
    stk.emplace(startSymbol);

    do {
        std::string top = stk.top();

        if (top == "$") {
            stk.pop();
            if (result.back() == '(') {
                result.pop_back(); // don't keep empty bracket
            } else {
                result.append(")");
            }
            continue;
        }
        std::string_view a = token.getRawData();
        std::string aStr(a);
        if (token.is(tok::eof)) {
            aStr = "#";
        }

        if (tSymbols.contains(top) || top == "#") {

            if (token.is(tok::eof) || a == top) {
                if (top != "#") {
                    stk.pop();
                    advance();

                    result.append(a);
                }
                continue;
            }


        } else {
            if (map.contains(top)) {
                if (map.at(top).contains(aStr)) {

                    stk.pop();
                    stk.emplace("$");
                    const auto &vec = map.at(top).at(aStr);
                    for (auto iter = vec.rbegin(); iter != vec.rend(); ++iter) {
                        stk.push(*iter);
                    }

                    result.append(top);
                    result.append("(");
                    continue;
                }
            }
        }

        error();
        return {};
    } while (stk.top() != "#");

    while (!stk.empty()) {
        result.append(stk.top());
        stk.pop();
    }

    result.pop_back(); // remove '#'

    return result;
}

}