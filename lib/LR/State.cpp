//
// Created by aojoie on 4/24/2023.
//

#include "LR/State.hpp"

#include <format>

namespace AN::LR {


std::ostream &AN::LR::operator<< (std::ostream &os, const AN::LR::State &state) {
    os << std::format("state id: {}", state.id) << '\n';
    for (const auto &item : state._items) {
        os << std::format("{} -> ", item.getLeft()->getVal());
        for (int i = 0; i < item.getDotPos(); ++i) {
            os << item.getRights()[i]->getVal() << ' ';
        }
        os << '.';
        for (int i = item.getDotPos(); i < item.getRights().size(); ++i) {
            os << ' ' << item.getRights()[i]->getVal();
        }
        os << '\n';
    }
    for (const auto &item : state.gotoMap) {
        os << std::format("GOTO( {}, {} )", item.first, item.second) << '\n';
    }
    return os;

}

}