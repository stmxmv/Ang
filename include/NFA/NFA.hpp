//
// Created by aojoie on 3/28/2023.
//

#ifndef ANG_NFA_HPP
#define ANG_NFA_HPP

#include "NFA/State.hpp"

#include <cstdint>
#include <vector>
#include <unordered_map>
#include <unordered_set>

namespace AN::NFA {

class NFA {

    StateManager &stateManager;

    uint32_t initialStatId;
    std::vector<uint32_t> acceptedStatIds;

public:

    /// \brief construct a new NFA instance with an newly-created initial state and no accepted states
    explicit NFA(StateManager &stateManager) : stateManager(stateManager) {
        initialStatId = stateManager.newState().getId();
    }

    State &getInitialState() const {
        return stateManager.getState(initialStatId);
    }

    void addAcceptedState(uint32_t id) {
        if (std::find(acceptedStatIds.begin(), acceptedStatIds.end(), id) == acceptedStatIds.end()) {
            acceptedStatIds.push_back(id);
        }
    }

    void removeAllAcceptedState() {
        acceptedStatIds.clear();
    }

    State &getFirstAcceptedState() {
        return stateManager.getState(acceptedStatIds.at(0));
    }

    bool isAcceptedState(uint32_t id) const {
        return std::find(acceptedStatIds.begin(), acceptedStatIds.end(), id) != acceptedStatIds.end();
    }

    const std::vector<uint32_t> &getAcceptedStatIds() const {
        return acceptedStatIds;
    }

    bool match(std::string_view str);

    std::string getPrettyString() const;
};




}



#endif//ANG_NFA_HPP
