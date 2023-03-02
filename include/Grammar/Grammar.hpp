//
// Created by aojoie on 3/1/2023.
//

#ifndef ANG_GRAMMER_HPP
#define ANG_GRAMMER_HPP

#include <string_view>
#include <vector>

namespace AN::grammar {

enum GrammarType : unsigned long {
    GrammarTypeUnknown  = 0,
    GrammarTypeRG       = 1,
    GrammarTypeRG_right = 1 << 1,
    GrammarTypeRG_left  = 1 << 2,
    GrammarTypeCFG      = 1 << 3,
    GrammarTypeCSG      = 1 << 4,
    GrammarTypePSG      = 1 << 5
};

class Symbol {
    std::string_view val;
public:
    enum ValueKind { Unknown, TSymbol, VSymbol };

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
    Production(const std::vector<Symbol> &leftSymbols,
               const std::vector<Symbol> &rightSymbols)
        : left_symbols(leftSymbols), right_symbols(rightSymbols) {}

    GrammarType getType() const;

};

class Grammar {

    std::vector<Production> products;

public:

    explicit Grammar(const std::vector<Production> &products) : products(products) {}

    GrammarType getType() const;

};


}

#endif//ANG_GRAMMER_HPP
