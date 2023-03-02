//
// Created by aojoie on 3/1/2023.
//

#include "Grammar/Parser.hpp"
#include <iostream>

namespace AN::grammar {


void Parser::error() {
    std::cout << "Unexpected" << token.getRawData() << '\n';
    hasError = true;
}

bool Parser::expect(tok::TokenKind tokenKind) {
    if (token.getKind() != tokenKind) {
        error();
        return false;
    }
    return true;
}

}