//
// Created by aojoie on 3/28/2023.
//

#ifndef ANG_REGEX_HPP
#define ANG_REGEX_HPP

#include "NFA/NFA.hpp"
#include <stack>
#include <iostream>

namespace AN::NFA {

class RegEx {

    StateManager &stateManager;

public:

    explicit RegEx(StateManager &stateManager) : stateManager(stateManager) {}

    std::string convertToPostfix(std::string_view str);

    std::unique_ptr<NFA> parse(std::string_view str);

};

}

#endif//ANG_REGEX_HPP
