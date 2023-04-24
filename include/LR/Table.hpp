//
// Created by aojoie on 4/24/2023.
//

#ifndef ANG_TABLE_HPP
#define ANG_TABLE_HPP

#include <Grammar/Grammar.hpp>

#include <ostream>
#include <string_view>
#include <unordered_map>

namespace AN::LR {

enum ActionType {
    kActionTypeShift = 0,
    kActionTypeReduce,
    kActionTypeAcc
};

struct Action {
    ActionType type;
    int        value;// ether state id or production id
};


struct Table {

    typedef std::unordered_map<int, std::unordered_map<std::string_view, Action>> ActionMap;
    typedef std::unordered_map<int, std::unordered_map<std::string_view, int>>    GotoMap;

    ActionMap actionMap;
    GotoMap   gotoMap;

};

std::ostream &operator<< (std::ostream &os, const Table &table);


}// namespace AN::LR

#endif//ANG_TABLE_HPP
