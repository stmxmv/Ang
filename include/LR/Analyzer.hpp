//
// Created by aojoie on 4/24/2023.
//

#ifndef ANG_ANALYZER_HPP
#define ANG_ANALYZER_HPP

#include <Grammar/Grammar.hpp>
#include <LR/Table.hpp>

#include <iostream>

namespace AN::LR {

class Analyzer {

    Table _table;

    const grammar::Grammar *_grammar;

public:

    Analyzer(Table table, const grammar::Grammar *grammar)
        : _table(std::move(table)), _grammar(grammar) {}

    void analyze(std::string_view str, std::ostream &os);


};

}

#endif//ANG_ANALYZER_HPP
