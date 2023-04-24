//
// Created by aojoie on 4/24/2023.
//

#include "LR/Parser.hpp"
#include <algorithm>
#include <ranges>

namespace AN::LR {

std::set<Item> Parser::getClosure(const Item &item) const {
    std::set<Item> closure{ item };
    if (!item.hasNext() || item.getNext()->getSymbolKind() == grammar::Symbol::TSymbol) {
        return closure;
    }

    const grammar::Symbol *symbol = item.getNext();

    auto productions = grammar->getProductions() |
                       std::views::filter([symbol](const grammar::Production *prop) {
                           return prop->getLeftSymbols().size() == 1 && prop->getLeftSymbols()[0] == symbol;
                       });
    for (const grammar::Production *prop : productions) {
        Item closureItem(prop->getLeftSymbols().front(), prop->getRightSymbols(), 0, prop);
        closure.merge(getClosure(closureItem));
    }

    return closure;
}


std::set<State> Parser::parseState() {

    std::set<State> states;

    auto hasState = [&states, this](const std::set<Item> &items) {
        auto it = std::ranges::find_if(states, [&](auto &&state) {
            return items == state.getItems();
        });
        if (it != std::ranges::end(states)) {
            return it->getID();
        }
        return -1;
    };

    int stateID = 0;

    State                  s0(stateID++);
    const grammar::Symbol *startSym = grammar->getGrammarTitle()->getStartSymbol();
    const grammar::Symbol *right[]  = { startSym };
    Item                   startItem(&startEx, right, 0, &productionEx);

    s0.addCommonItem(startItem);
    s0.mergeItems(getClosure(startItem));

    states.insert(s0);

    for (const State &state : states) {
        auto items = state.getItems() |
                     std::views::filter([](auto &&item) {
                         return item.hasNext();
                     });

        if (items.empty()) continue;

//        std::vector<Item> sortedItems(items.begin(), items.end());
//
//        std::ranges::stable_sort(sortedItems, [](auto &&a, auto &&b) {
//            return a.getNextVal() < b.getNextVal();
//        });
//
//        auto commonItemsGroups = sortedItems | std::views::chunk_by([](auto &&a, auto &&b) {
//                                     return a.getNextVal() == b.getNextVal();
//                                 });

        for (auto &&item : items) {

            auto commonItems = items | std::views::filter([&item](auto &&item1) {
                                    return item1.getNextVal() == item.getNextVal();
                               });

            auto nextVal = commonItems.front().getNextVal();

            auto nextCommonItems = commonItems | std::views::transform([](auto item1) {
                                        item1.setDotPos(item1.getDotPos() + 1);
                                        return item1;
                                   });

            std::set<Item> nextItems;
            for (auto &&i : nextCommonItems) {
                nextItems.merge(getClosure(i));
            }

            if (int id = hasState(nextItems); id != -1) {
                state.addGoto(nextVal, id);
                continue;
            }

            State newState(stateID++);

            for (auto &&commonItem : nextCommonItems) {
                newState.addCommonItem(commonItem);
            }

            newState.mergeItems(std::move(nextItems));

            state.addGoto(nextVal, newState.getID());

            states.insert(newState);
        }

    }

    return states;
}

Table Parser::parseTable() {
    std::set<State> states = parseState();

    Table table;

    for (const auto &state : states) {
        for (const auto &item : state.getItems()) {
            if (item.hasNext()) {
                if (item.getNext()->getSymbolKind() == grammar::Symbol::TSymbol) {
                    Action action{};
                    action.type = kActionTypeShift;
                    action.value = state.gotoState(item.getNext()->getVal());
                    table.actionMap[state.getID()][item.getNext()->getCharLiteral()] = action;
                } else {
                    /// V Symbol
                    table.gotoMap[state.getID()][item.getNextVal()] = state.gotoState(item.getNextVal());
                }
            } else {
                /// acc
                if (item.getLeft() == &startEx && item.getDotPos() == 1) {
                    Action action{};
                    action.type = kActionTypeAcc;
                    table.actionMap[state.getID()]["#"] = action;
                } else {
                    Action action{};
                    action.type = kActionTypeReduce;
                    action.value = item.getProduction()->getID();

                    table.actionMap[state.getID()]["#"] = action;
                    for (const grammar::Symbol *tSymbol : grammar->getAllTSymbols()) {
                        table.actionMap[state.getID()][tSymbol->getCharLiteral()] = action;
                    }
                }
            }
        }
    }


    return table;
}

}// namespace AN::LR