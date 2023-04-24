//
// Created by aojoie on 3/1/2023.
//

#include "Grammar/Parser.hpp"
#include <iostream>
#include <unordered_set>

namespace AN::grammar {

#define SYMBOL_TOKEN_TYPE tok::identifier, tok::char_literal, tok::string_literal, tok::float_literal, tok::integer_literal

inline static Symbol::ValueType toSymbolValueType(tok::TokenKind kind) {
    switch (kind) {
        case tok::identifier:
            return Symbol::Identifier;
        case tok::char_literal:
            return Symbol::CharLiteral;
        case tok::string_literal:
            return Symbol::StringLiteral;
        case tok::float_literal:
            return Symbol::FloatLiteral;
        case tok::integer_literal:
            return Symbol::IntegerLiteral;
        default:
            return Symbol::UnknownValueType;
    }
}

GrammarTitle *Parser::parseGrammarTitle() {
    std::string_view grammar_name;
    Symbol *startSymbol = nullptr;
    GrammarTitle *grammarTitle = nullptr;
    /// parse grammar name and start symbol
    if (expect(tok::identifier)) {
        grammar_name = token.getRawData();
        advance();
        if (consume(tok::left_bracket)) {
            if (expect(tok::identifier)) {
                startSymbol = new(context) Symbol(token.getRawData(), Symbol::VSymbol, toSymbolValueType(token.getKind()));
                symbol_map[token.getRawData()] = startSymbol;
                advance();
                if (!consume(tok::right_bracket) || !consume(tok::delimiter)) {
                    goto __error;
                }
            } else {
                goto __error;
            }
        } else {
            goto __error;
        }
    } else {
        goto __error;
    }

    grammarTitle = new(context) GrammarTitle(grammar_name, startSymbol);
    return grammarTitle;

__error:
    while (token.isNot(tok::eof)) {
        advance();
    }
    return nullptr;
}

std::vector<Production *> Parser::parseProduction() {
    ++productionID;
    std::vector<Production *> productions;
    std::vector<Symbol *> pro_left_symbols, pro_right_symbols;

    if (!expectOneOf(SYMBOL_TOKEN_TYPE)) {
        goto __error;
    }

    /// parse left side of the production
    while (token.isOneOf(SYMBOL_TOKEN_TYPE)) {
        Symbol *sym = getSymbol(token.getRawData());
        if (!sym) {
            sym = new (context) Symbol(token.getRawData(), token.is(tok::identifier) ? Symbol::VSymbol : Symbol::TSymbol,
                                                                  toSymbolValueType(token.getKind()));
            symbol_map[token.getRawData()] = sym;
        }

        pro_left_symbols.push_back(sym);
        advance();
    }

    /// separator
    if (!consume(tok::separator)) {
        goto __error;
    }

    /// right side can have no symbols ( epsilon )
    if (token.is(tok::alter)) {
        productions.push_back(new(context) Production(productionID, pro_left_symbols, {}));
        advance();
    }

    while (token.isOneOf(SYMBOL_TOKEN_TYPE)) {
        Symbol *sym = getSymbol(token.getRawData());
        if (!sym) {
            // currently the symbol is whether V or T  is unknown
            sym = new (context) Symbol(token.getRawData(), token.is(tok::identifier) ? Symbol::VSymbol : Symbol::TSymbol,
                                                                  toSymbolValueType(token.getKind()));
            symbol_map[token.getRawData()] = sym;
        }
        pro_right_symbols.push_back(sym);
        advance();

        if (token.is(tok::alter)) {
            Production *production = new(context) Production(productionID, pro_left_symbols, pro_right_symbols);
            productions.push_back(production);
            pro_right_symbols.clear();
            advance();

            /// right side can have no symbols ( epsilon )
            while (token.is(tok::alter)) {
                productions.push_back(new(context) Production(productionID, pro_left_symbols, {}));
                advance();
            }
        }
    }

    if (!consume(tok::delimiter)) {
        goto __error;
    }

    productions.push_back(new(context) Production(productionID, pro_left_symbols, pro_right_symbols));

    return productions;

__error:
    while (token.isNot(tok::eof)) {
        advance();
    }
    return {};
}

Grammar *Parser::parse() {
    GrammarTitle *grammarTitle = parseGrammarTitle();

    std::vector<Production *> products;
    while (token.isNot(tok::eof)) {
        std::vector<Production *> parsedProducts = parseProduction();
        products.insert(products.end(), parsedProducts.begin(), parsedProducts.end());
    }

    /// construct Grammar
    Grammar *grammar = new (context) Grammar(grammarTitle, products, symbol_map);

    return grammar;
}

AST *Parser::parseAST() {
    /// we do not use the grammar title
    GrammarTitle *grammarTitle [[maybe_unused]] = parseGrammarTitle();

    if (!expect(tok::identifier)) {
        std::cout << "Not start symbol provided" << std::endl;
        return nullptr;
    }

    VSymbol *root = new (context) VSymbol(token.getRawData());

    std::unordered_map<std::string_view, VSymbol*> symbolMap;
    symbolMap[root->getName()] = root;

    while (token.isNot(tok::eof)) {
        if (!expect(tok::identifier)) {
            goto __error;
        }

        if (!symbolMap.contains(token.getRawData())) {
            std::cout << "Grammar syntax error: Undefined V Symbol " << token.getRawData() << std::endl;
            goto __error;
        }

        VSymbol *vSymbol = symbolMap[token.getRawData()];
        advance();

        if (!consume(tok::separator)) {
            goto __error;
        }

        while (token.isOneOf(SYMBOL_TOKEN_TYPE)) {
            if (token.is(tok::identifier)) {
                /// V symbol
                VSymbol *symbol = new (context) VSymbol(token.getRawData());
                symbolMap[symbol->getName()] = symbol;
                vSymbol->addChild(symbol);
            } else {
                /// T symbol
                TSymbol *symbol = new (context) TSymbol(token.getRawData());
                vSymbol->addChild(symbol);
            }
            advance();
        }

        if (!consume(tok::delimiter)) {
            goto __error;
        }
    }

    return root;

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