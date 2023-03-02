//
// Created by aojoie on 3/1/2023.
//

#ifndef ANG_GRAMMER_HPP
#define ANG_GRAMMER_HPP

#include <string_view>
#include <utility>
#include <vector>

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

class Symbol {
    std::string_view val;
public:
    enum ValueKind { Unknown, TSymbol, VSymbol };

    Symbol() : valueKind(Unknown) {}

    Symbol(std::string_view val, ValueKind valueKind) : val(val), valueKind(valueKind) {}

    const std::string_view &getVal() const {
        return val;
    }

    ValueKind getKind() const {
        return valueKind;
    }

private:
    ValueKind valueKind;
};

class Production {
    std::vector<Symbol> left_symbols;
    std::vector<Symbol> right_symbols;

public:
    Production(std::vector<Symbol> leftSymbols,
               std::vector<Symbol> rightSymbols)
        : left_symbols(std::move(leftSymbols)), right_symbols(std::move(rightSymbols)) {}

    GrammarType getType() const;

};

class GrammarTitle {
    std::string_view name;
    Symbol start_symbol;
public:
    GrammarTitle(const std::string_view &name, const Symbol &startSymbol) : name(name), start_symbol(startSymbol) {}

    const std::string_view &getName() const {
        return name;
    }
    const Symbol &getStartSymbol() const {
        return start_symbol;
    }
};

class Grammar {

    std::vector<Production> products;


public:

    explicit Grammar(std::vector<Production> products) : products(std::move(products)) {}

    GrammarType getType() const;

};


}

#endif//ANG_GRAMMER_HPP
