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

        if (left_symbols.size() == 1 && left_symbols.front()->getSymbolKind() == Symbol::VSymbol) {

            type =  GrammarTypeCFG;

            if (right_symbols.empty()) {
                /// epsilon

                type = GrammarTypeRG;

            } else  {
                // check RG
                if (std::all_of(right_symbols.begin(), right_symbols.end(), [](auto &&symbol) {
                        return symbol->getSymbolKind() == Symbol::TSymbol;
                    })) {
                    type = GrammarTypeRG;
                }

                if (right_symbols.front()->getSymbolKind() == Symbol::VSymbol) {

                    if (std::all_of(right_symbols.begin() + 1, right_symbols.end(), [](auto &&symbol) {
                            return symbol->getSymbolKind() == Symbol::TSymbol;
                        })) {
                        type = GrammarTypeRG_left;
                    }

                } else if (right_symbols.back()->getSymbolKind() == Symbol::VSymbol) {
                    if (std::all_of(right_symbols.begin(), right_symbols.end() - 1, [](auto &&symbol) {
                            return symbol->getSymbolKind() == Symbol::TSymbol;
                        })) {
                        type =  GrammarTypeRG_right;
                    }
                }
            }
        }
    }

    return type;
}

GrammarType Grammar::getType() const {
    uint64_t type = GrammarTypeUnknown;
    for (const Production *production : products) {
        type |= production->getType();
    }

    uint64_t flag = GrammarTypePSG;

    while (!(type & flag)) {
        flag <<= 1;
    }

    if ((flag & GrammarTypeRG_right) && (type & GrammarTypeRG_left)) {
        return GrammarTypeCFG;
    }


    return (GrammarType)flag;
}

std::string Production::getPrettyString() const {
    std::string result;
    // left
    for (const Symbol *symbol : left_symbols) {
        result.append(symbol->getVal());
        result.push_back(' ');
    }

    result.append("-> ");

    // right
    for (const Symbol *symbol : right_symbols) {
        result.append(symbol->getVal());
        result.push_back(' ');
    }

    result.back() = ';';

    return result;
}

std::string GrammarTitle::getPrettyString() const {
    std::string result;

    result.append(name);
    result.push_back('[');
    result.append(start_symbol->getVal());
    result.append("];");

    return result;
}


std::string Grammar::getPrettyString() const {
    std::string result;
    result.append(grammarTitle->getPrettyString());
    result.push_back('\n');

    for (const Production *production : products) {
        result.append(production->getPrettyString());
        result.push_back('\n');
    }

    return result;
}

}