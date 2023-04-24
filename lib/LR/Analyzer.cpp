//
// Created by aojoie on 4/24/2023.
//

#include "LR/Analyzer.hpp"

#include <iostream>
#include <stack>
#include <deque>

namespace AN::LR {

using std::endl;

void Analyzer::analyze(std::string_view str, std::ostream &os) {
    std::vector<int>         stateStack;
    std::vector<std::string> symbolStack;

    stateStack.push_back(0);
    symbolStack.emplace_back("#");

    std::string input(str);
    input.push_back('#');

    int ip = 0;

    os << "Stack input" << '\n';

    std::vector<std::string> astStack;

    for (;;) {

        for (int state : stateStack) {
            os << state;
        }
        for (auto && sym : symbolStack) {
            os << sym;
        }

        os << ' ';

        os << input.substr(ip);

        int state = stateStack.back();
        std::string_view a = std::string_view(input).substr(ip, 1);

        if (!_table.actionMap[state].contains(a)) {
            os << "Input Error" << endl;
            return;
        }

        Action action = _table.actionMap[state][a];
        if (action.type == kActionTypeAcc) {
            os << " accepted" << "\n\n";
            os << astStack.front() << endl;
            return;
        }

        if (action.type == kActionTypeShift) {
            symbolStack.emplace_back(a);
            stateStack.push_back(action.value);
            ++ip;

            astStack.emplace_back(a);

            os << " shift to state " << action.value << " and input " << a;

        } else if (action.type == kActionTypeReduce) {
            int productionID = action.value;
            const grammar::Production *production = _grammar->getProduction(productionID);

            int count = production->getRightSymbols().size();

            std::string reduce(production->getLeftSymbols()[0]->getVal());
            reduce.append("(");
            for (int i = astStack.size() - count; i < astStack.size(); ++i) {
                reduce.append(astStack[i]);
            }
            reduce.append(")");
            astStack.resize(astStack.size() - count);
            astStack.push_back(reduce);

            stateStack.resize(stateStack.size() - count);
            symbolStack.resize(symbolStack.size() - count);

            int curState = stateStack.back();
            symbolStack.emplace_back(production->getLeftSymbols()[0]->getVal());
            stateStack.push_back(_table.gotoMap[curState][production->getLeftSymbols()[0]->getVal()]);

            os << " reduce " << production->getPrettyString();

        } else {
            assert(false && "Unknown action type");
        }

        os << '\n';

    }
}

}// namespace AN::LR