//
// Created by aojoie on 4/24/2023.
//

#include "LR/Table.hpp"

#include <unordered_set>
#include <format>

namespace AN::LR {

std::ostream &operator<< (std::ostream &os, const Table &table) {


    std::unordered_set<std::string_view> actionCols, gotoCols;

    for (auto &&i : table.actionMap | std::views::values) {
        for (auto &&j : i | std::views::keys) {
            actionCols.insert(j);
        }
    }

    for (auto &&i : table.gotoMap | std::views::values) {
        for (auto &&j : i | std::views::keys) {
            gotoCols.insert(j);
        }
    }

    os << "actionMap" << '\n';
    os << "id";
    for (auto &&col : actionCols) {
        os << ' ' << col;
    }
    os << '\n';
    for (auto &&[id, map] : table.actionMap) {
        os << id << ' ';
        for (auto &&col : actionCols) {
            if (map.contains(col)) {
                Action action = map.at(col);
                switch (action.type) {
                    case kActionTypeShift:
                        os << 'S' << action.value;
                        break;
                    case kActionTypeReduce:
                        os << 'R' << action.value;
                        break;
                    case kActionTypeAcc:
                        os << "Acc";
                        break;
                }
            } else {
                os << ' ';
            }
            os << ' ';
        }
        os << '\n';
    }

    os << '\n';
    os << "gotoMap" << '\n';
    os << "id";
    for (auto &&col : gotoCols) {
        os << ' ' << col;
    }
    os << '\n';
    for (auto &&[id, map] : table.gotoMap) {
        os << id;
        for (auto &&col : gotoCols) {
            if (map.contains(col)) {
                os << ' ' << map.at(col);
            } else {
                os << ' ';
            }
        }
        os << '\n';
    }

    return os;
}
}