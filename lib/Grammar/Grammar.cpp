//
// Created by aojoie on 3/1/2023.
//

#include "Grammar/Grammar.hpp"
#include <algorithm>

namespace AN::grammar {


GrammarType grammar::Production::getType() const {
    GrammarType type = GrammarTypePSG;
    if (left_symbols.size() <= right_symbols.size()) {
        type =  GrammarTypeCSG;
    }

    if (left_symbols.size() == 1 && left_symbols.front().getKind() == Symbol::VSymbol) {
        type =  GrammarTypeCFG;

        if (!right_symbols.empty()) {
            // check RG
            if (std::all_of(right_symbols.begin(), right_symbols.end(), [](auto &&symbol) {
                    return symbol.getKind() == Symbol::TSymbol;
                })) {
                type = GrammarTypeRG;
            }

            if (right_symbols.front().getKind() == Symbol::VSymbol) {

                if (std::all_of(right_symbols.begin() + 1, right_symbols.end(), [](auto &&symbol) {
                        return symbol.getKind() == Symbol::TSymbol;
                    })) {
                    type = GrammarTypeRG_left;
                }

            } else if (right_symbols.back().getKind() == Symbol::VSymbol) {
                if (std::all_of(right_symbols.begin(), right_symbols.end() - 1, [](auto &&symbol) {
                        return symbol.getKind() == Symbol::TSymbol;
                    })) {
                    type =  GrammarTypeRG_right;
                }
            }
        }
    }

    return type;
}

GrammarType Grammar::getType() const {
    uint64_t type = GrammarTypeUnknown;
    for (const Production &production : products) {
        type |= production.getType();
    }

    uint64_t flag = GrammarTypePSG;

    while (!(type & flag)) {
        flag <<= 1;
    }

    if (flag & (GrammarTypeRG | GrammarTypeRG_left | GrammarTypeRG_right)) {
        if ((type & GrammarTypeRG_left) && (type & GrammarTypeRG_right)) {
            return GrammarTypeCFG;
        }
    }


    return (GrammarType)flag;
}

}