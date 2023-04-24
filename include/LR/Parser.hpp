//
// Created by aojoie on 4/24/2023.
//

#ifndef ANG_LR_PARSER_HPP
#define ANG_LR_PARSER_HPP

#include <Grammar/Grammar.hpp>
#include <LR/State.hpp>
#include <LR/Table.hpp>
#include <set>

namespace AN::LR {

class Parser {

    grammar::Grammar *grammar;

    grammar::Symbol startEx;
    grammar::Production productionEx;

    std::set<Item> getClosure(const Item &item) const;

public:

    explicit Parser(grammar::Grammar *aGrammar)
        : grammar(aGrammar),
          startEx("S'", grammar::Symbol::VSymbol, grammar::Symbol::Identifier),
          productionEx(0, { &startEx }, {}){}

    std::set<State> parseState();

    Table parseTable();

};

}

#endif//ANG_LR_PARSER_HPP
