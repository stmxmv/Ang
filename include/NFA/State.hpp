//
// Created by aojoie on 3/28/2023.
//

#ifndef ANG_STATE_HPP
#define ANG_STATE_HPP

#include "Basic/typedef.h"

#include <string>
#include <cstdint>
#include <vector>
#include <unordered_map>

namespace AN::NFA {

struct Transition {
    std::string   input;
    uint32_t nextStatId;

    bool operator==(const Transition &) const = default;
};


class State : private NonCopyable {

    uint32_t id;
    std::string rawData;
    std::vector<Transition> transitions;

    class StateManager &stateManager;
    friend class StateManager;

    explicit State(StateManager &stateManager, uint32_t id, std::string_view rawData = {})
        : id(id), rawData(rawData), stateManager(stateManager) {}

    template<typename ...Args>
    static std::unique_ptr<State> Alloc(Args &&...args) {
        return std::unique_ptr<State>(new State(std::forward<Args>(args)...));
    }

public:

    void addTransition(std::string_view input, uint32_t nextStatId) {
        auto iter = std::find_if(transitions.begin(), transitions.end(), [=] (auto &&trans) {
            return trans.input == input && trans.nextStatId == nextStatId;
        });

        if (iter == transitions.end()) {
            transitions.push_back({ std::string{ input }, nextStatId });
        }
    }

    uint32_t getId() const {
        return id;
    }

    std::vector<uint32_t> getNextStateId(std::string_view input) {
        std::vector<uint32_t> result;
        for (const auto &trans : transitions) {
            if (trans.input == input) {
                result.push_back(trans.nextStatId);
            }
        }
        return result;
    }

    const std::vector<Transition> &getTransitions() const {
        return transitions;
    }

    const char *getRawData() const {
        return rawData.c_str();
    }

    void setRawData(std::string_view str);
};


class StateManager : private NonCopyable {

    std::vector<std::unique_ptr<State>> states; // state.id is the index of this array
    std::unordered_map<std::string_view, uint32_t> stringStateIdMap;

public:

    State &getState(uint32_t id) {
        return *states[id];
    }

    State &getState(std::string_view statString) {
        if (auto iter = stringStateIdMap.find(statString);
            iter != stringStateIdMap.end()) {
            return *states[iter->second];
        }
        states.emplace_back(State::Alloc(*this, (uint32_t)states.size(), statString));
        stringStateIdMap[statString] = states.size() - 1;
        return *states.back();
    }

    State &newState() {
        states.emplace_back(State::Alloc(*this, (uint32_t)states.size()));
        return *states.back();
    }

    void associateStateWithString(uint32_t id, std::string_view str) {
        stringStateIdMap[str] = id;
    }

    uint32_t getStateNum() const {
        return (uint32_t) states.size();
    }

    const std::vector<std::unique_ptr<State>> &getStates() const {
        return states;
    }

};


}

#endif//ANG_STATE_HPP
