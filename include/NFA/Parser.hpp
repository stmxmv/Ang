//
// Created by aojoie on 3/28/2023.
//

#ifndef ANG_PARSER_HPP
#define ANG_PARSER_HPP

#include "NFA/Lexer.hpp"
#include "NFA/NFA.hpp"

#include <memory>

namespace AN::NFA {

class Parser {

    bool hasError;
    bool didSetInitialState = false;
    Lexer &lexer;
    StateManager &stateManager;
    Token token;

    void advance() {
        lexer.next(token);
    }

    void error();

    bool expect(tok::TokenKind tokenKind);

    bool expectOneOf(tok::TokenKind K1) {
        return expect(K1);
    }

    template<typename... Ts>
    bool expectOneOf(tok::TokenKind K1, Ts... Ks) {
        if (token.isOneOf(K1, Ks...)) {
            return true;
        }
        error();
        return false;
    }

    bool consume(tok::TokenKind tokenKind) {
        if (!expect(tokenKind)) {
            return false;
        }
        advance();
        return true;
    }

    bool parseTransition(NFA &nfa);

    bool parseAcceptedState(NFA &nfa);

public:
    explicit Parser(Lexer &aLexer, StateManager &stateManager)
        : lexer(aLexer), stateManager(stateManager), hasError() {
        advance();
    }

    std::unique_ptr<NFA> parse();

};


}

#endif//ANG_PARSER_HPP
