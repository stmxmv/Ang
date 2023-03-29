//
// Created by aojoie on 3/28/2023.
//

#include "NFA/Parser.hpp"
#include <iostream>

namespace AN::NFA {


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

#define GUARD(stmt) \
    if (!(stmt)) { return false; }

bool Parser::parseTransition(NFA &nfa) {
    advance(); // go through 'f'
    GUARD(consume(tok::left_bracket));
    GUARD(expect(tok::identifier));

    std::string_view const statString = token.getRawData();


    /// assume initial state is 'S'
    if (!didSetInitialState) {
        nfa.getInitialState().setRawData(statString);
        didSetInitialState = true;
    } else {
        State  const&stat = stateManager.getState(statString);
//        nfa.addStateIfNotExist(stat.getId());
    }

    advance();

    GUARD(consume(tok::comma));

    GUARD(expect(tok::identifier));
    std::string_view const input = token.getRawData();
//    nfa.inputs.insert(input);
    advance();

    GUARD(consume(tok::right_bracket));
    GUARD(consume(tok::assign));
    GUARD(expect(tok::identifier));

    std::string_view const nextStatString = token.getRawData();

    State &stat = stateManager.getState(statString);
    State  const& nextStat = stateManager.getState(nextStatString);
//    nfa.addStateIfNotExist(nextStat.getId());
    stat.addTransition(input, nextStat.getId());

    advance();

    return true;
}

bool Parser::parseAcceptedState(NFA &nfa) {
    advance(); // go through '{'

    GUARD(expect(tok::identifier));

    bool first = true;
    while (token.is(tok::identifier)) {
        if (first) {
            first = false;
        } else {
            GUARD(consume(tok::comma));
        }

        nfa.addAcceptedState(stateManager.getState(token.getRawData()).getId());

        advance();
    }

    GUARD(consume(tok::right_curly_bracket));

    return true;
}

#undef GUARD

std::unique_ptr<NFA> Parser::parse() {
    std::unique_ptr<NFA> nfa = std::make_unique<NFA>(stateManager);

    while (token.isNot(tok::eof)) {
        if (token.is(tok::fun_beg)) {
            if (!parseTransition(*nfa)) {
                goto __error;
            }

            if (!consume(tok::comma)) {
                goto __error;
            }

        } else if (token.is(tok::left_curly_bracket)) {
            if (!parseAcceptedState(*nfa)) {
                goto __error;
            }
        }
    }

    return nfa;

__error:
    while (token.isNot(tok::eof)) {
        advance();
    }
    return nullptr;
}

}