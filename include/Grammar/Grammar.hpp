//
// Created by aojoie on 3/1/2023.
//

#ifndef ANG_GRAMMER_HPP
#define ANG_GRAMMER_HPP

#include "Grammar/ASTContext.hpp"

#include <string_view>
#include <utility>
#include <vector>
#include <unordered_map>

namespace AN::grammar {



enum GrammarType : unsigned long {
    GrammarTypeUnknown  = 0,
    GrammarTypePSG      = 1 << 0,
    GrammarTypeCSG      = 1 << 1,
    GrammarTypeCFG      = 1 << 2,
    GrammarTypeRG_right = 1 << 3,
    GrammarTypeRG_left  = 1 << 4,
    GrammarTypeRG       = 1 << 5,
};

class Symbol : public ASTAllocated<Symbol> {
    std::string_view val;
public:
    enum SymbolKind { UnknownSymbol, TSymbol, VSymbol };

    enum ValueType { UnknownValueType, Identifier, CharLiteral, StringLiteral, FloatLiteral, IntegerLiteral };

    Symbol() : symbolKind(UnknownSymbol), valueType(UnknownValueType) {}

    Symbol(std::string_view val, SymbolKind valueKind, ValueType valueType)
        : val(val), symbolKind(valueKind), valueType(valueType) {}

    const std::string_view &getVal() const {
        return val;
    }

    SymbolKind getSymbolKind() const {
        return symbolKind;
    }

    ValueType getValueType() const {
        return valueType;
    }

    void setValueKind(SymbolKind aValueKind) {
        symbolKind = aValueKind;
    }

private:
    SymbolKind symbolKind;
    ValueType valueType;
};

class Production : public ASTAllocated<Production> {
    std::vector<Symbol *> left_symbols;
    std::vector<Symbol *> right_symbols;

public:
    Production(std::vector<Symbol *> leftSymbols,
               std::vector<Symbol *> rightSymbols)
        : left_symbols(std::move(leftSymbols)), right_symbols(std::move(rightSymbols)) {}

    GrammarType getType() const;

    const std::vector<Symbol *> &getLeftSymbols() const {
        return left_symbols;
    }
    const std::vector<Symbol *> &getRightSymbols() const {
        return right_symbols;
    }

    std::string getPrettyString() const;

};

class GrammarTitle : public ASTAllocated<GrammarTitle> {
    std::string_view name;
    Symbol *start_symbol;
public:
    GrammarTitle(const std::string_view &name, Symbol *startSymbol) : name(name), start_symbol(startSymbol) {}

    const std::string_view &getName() const {
        return name;
    }
    const Symbol *getStartSymbol() const {
        return start_symbol;
    }

    std::string getPrettyString() const;
};

class Grammar : public ASTAllocated<Grammar> {
    GrammarTitle *grammarTitle;
    std::vector<Production *> products;
    std::unordered_map<std::string_view, Symbol *> symbol_map;
public:

    Grammar(GrammarTitle *grammarTitle,
            std::vector<Production *> products,
            std::unordered_map<std::string_view, Symbol *> symbol_map)
        : grammarTitle(grammarTitle), products(std::move(products)), symbol_map(std::move(symbol_map)) {}

    GrammarType getType() const;

    GrammarTitle *getGrammarTitle() const { return grammarTitle; }

    Symbol *getSymbolByVal(std::string_view val) {
        if (auto iter = symbol_map.find(val); iter != symbol_map.end()) {
            return iter->second;
        }
        return nullptr;
    }

    const std::vector<Production *> &getProductions() const {
        return products;
    }

    std::string getPrettyString() const;
};


}

#endif//ANG_GRAMMER_HPP
