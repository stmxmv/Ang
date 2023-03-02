//
// Created by aojoie on 3/1/2023.
//

#include "Grammar/Grammar.hpp"
#include <algorithm>

namespace AN::grammar {


GrammarType grammar::Production::getType() const {
    GrammarType type = GrammarTypePSG;

    if (left_symbols.size() <= right_symbols.size()) {
        type = GrammarTypeCSG;
    }

    if (left_symbols.size() == 1 && left_symbols.front().getKind() == Symbol::VSymbol) {
        type = GrammarTypeCFG;

        if (!right_symbols.empty()) {
            // check RG
            if (std::all_of(right_symbols.begin(), right_symbols.end(), [](auto &&symbol) {
                    return symbol.getKind() == Symbol::TSymbol;
                })) {

            }

            if (right_symbols.front().getKind() == Symbol::VSymbol) {


            } else if (right_symbols.back().getKind() == Symbol::VSymbol) {


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

    if ((type & GrammarTypeRG_left) && (type & GrammarTypeRG_right)) {
        return GrammarTypeCFG;
    }

    uint64_t flag = 1;
    while (!(type & flag)) {
        flag <<= 1;
    }
    return (GrammarType)flag;
}

}