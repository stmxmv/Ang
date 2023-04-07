//
// Created by aojoie on 3/28/2023.
//

#include "NFA/State.hpp"

namespace AN::NFA {

void State::setRawData(std::string_view str) {
    stateManager.associateStateWithString(id, str);
}


}