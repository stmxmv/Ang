//
// Created by aojoie on 3/29/2023.
//

#include "NFA/RegEx.hpp"

#include <unordered_set>

namespace AN::NFA {

namespace charinfo {

inline bool isASCII(char Ch) {
    return static_cast<unsigned char>(Ch) <= 127;
}


inline bool isAlNum(char Ch) {
    return isASCII(Ch) && (Ch >= 'A' && Ch <= 'Z') ||
           (Ch >= 'a' && Ch <= 'z') || (Ch >= '0' && Ch <= '9');
}

inline bool isValid(char ch) {
    return isAlNum(ch) || ch == '(' || ch == '|' || ch == ')' || ch == '*';
}

inline int getPriority(char op) {
    switch (op) {
        case '(':
            return 0;
        case '|':
            return 1;
        case '.':
            return 2;
        case '*':
            return 3;
        default:
            return -1;
    }
}

}// namespace charinfo

namespace {

struct neighbor {
    char a, b;
    bool operator == (const neighbor &) const = default;
};

}

}

template<>
struct std::hash<AN::NFA::neighbor> {
    size_t operator()(const AN::NFA::neighbor &item) const {
        return (std::hash<char>()(item.a) << 8) | std::hash<char>()(item.b);
    }
};

namespace AN::NFA {

static std::string preprocess_regex(std::string_view str) {
    if (str.empty()) { return {}; }

    /// check bracket match
    int bracketNum = 0;
    for (const char ch : str) {
        if (!charinfo::isValid(ch)) {
            return {};
        }
        if (ch == '(') {
            ++bracketNum;
        } else if (ch == ')') {
            --bracketNum;
        }
        if (bracketNum < 0) {
            return {};
        }
    }

    if (bracketNum != 0) {
        return {};
    }

    /// insert connect symbol '.'

    auto code = [](char ch) -> char {
        if (charinfo::isAlNum(ch)) {
            return 'a';
        }
        return ch;
    };


    const static std::unordered_set<neighbor> neighborSet {
            { 'a', '(' }, { 'a', 'a' }, { ')', 'a' },
            { '*', 'a' }, { '*', '(' }, { ')', '(' }
    };

    std::string result(str);
    for (int i = 0; i < result.length() - 1; ++i) {
        char const a = code(result[i]), b = code(result[i + 1]);

        neighbor const nb{ a, b };
        if (neighborSet.find(nb) != neighborSet.end()) {
            result.insert(i + 1, ".");
            ++i;
        }
    }

    return result;
}


static void convertToPostfix(std::string_view str, std::string &result) {
    std::string const preprocessed = preprocess_regex(str);
    if (preprocessed.empty()) { return; }

    std::stack<char> stk;
    for (char const ch : preprocessed) {
        if (charinfo::isAlNum(ch)) {
            result.push_back(ch);

        } else {

            if (stk.empty() || ch == '(') {

                stk.push(ch);

            } else if (ch == ')') {

                while (!stk.empty() && stk.top() != '(') {
                    result.push_back(stk.top());
                    stk.pop();
                }
                if (stk.empty()) {
                    goto __error;
                }
                stk.pop(); // pop '('

            } else {

                while (charinfo::getPriority(ch) <= charinfo::getPriority(stk.top())) {
                    result.push_back(stk.top());
                    stk.pop();
                    if (stk.empty()) {
                        break;
                    }
                }

                stk.push(ch);
            }
        }
    }

    while (!stk.empty()) {
        result.push_back(stk.top());
        stk.pop();
    }


    return;

__error:
    std::cout << "Regex grammar error" << std::endl;
//    return {};
}

std::unique_ptr<NFA> RegEx::parse(std::string_view raw_str) {
    std::string str;
    convertToPostfix(raw_str, str);

    std::stack<NFA> nfaStack;

    if (str.empty()) {
        goto __error;
    }

    for (int i = 0; i < (int) str.length(); ++i) {
        if (charinfo::isAlNum(str[i])) {
            NFA subNfa(stateManager);
            State &state = stateManager.newState();
            subNfa.getInitialState().addTransition(std::string(1, str[i]), state.getId());
            subNfa.addAcceptedState(state.getId());

            nfaStack.push(subNfa);

        } else if (str[i] == '.') {
            // connect
            if (nfaStack.size() < 2) {
                goto __error;
            }
            NFA nfar = nfaStack.top(); nfaStack.pop();
            NFA nfal = nfaStack.top(); nfaStack.pop();

            nfal.getFirstAcceptedState().addTransition("ε", nfar.getInitialState().getId());
            nfal.removeAllAcceptedState();
            nfal.addAcceptedState(nfar.getFirstAcceptedState().getId());

            nfaStack.push(nfal);

        } else if (str[i] == '*') {

            if (nfaStack.empty()) {
                goto __error;
            }

            NFA subNfa = nfaStack.top();
            nfaStack.pop();

            NFA newNfa(stateManager);
            State  const& acceptedState = stateManager.newState();
            newNfa.addAcceptedState(acceptedState.getId());
            newNfa.getInitialState().addTransition("ε", acceptedState.getId());
            newNfa.getInitialState().addTransition("ε", subNfa.getInitialState().getId());
            subNfa.getFirstAcceptedState().addTransition("ε", subNfa.getInitialState().getId());
            subNfa.getFirstAcceptedState().addTransition("ε", acceptedState.getId());
            nfaStack.push(newNfa);

        } else if (str[i] == '|') {

            if (nfaStack.size() < 2) {
                goto __error;
            }

            NFA nfar = nfaStack.top(); nfaStack.pop();
            NFA nfal = nfaStack.top(); nfaStack.pop();

            NFA newNFA(stateManager);
            newNFA.getInitialState().addTransition("ε", nfal.getInitialState().getId());
            newNFA.getInitialState().addTransition("ε", nfar.getInitialState().getId());

            State &acceptedState = stateManager.newState();
            nfal.getFirstAcceptedState().addTransition("ε", acceptedState.getId());
            nfar.getFirstAcceptedState().addTransition("ε", acceptedState.getId());

            newNFA.addAcceptedState(acceptedState.getId());

            nfaStack.push(newNFA);

        } else {
            goto __error;
        }
    }

    if (nfaStack.size() != 1) {
        goto __error;
    }

    return std::make_unique<NFA>(nfaStack.top());

__error:
    std::cout << "Parse Regex Error" << std::endl;
    return nullptr;
}
}