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

    mutable std::vector<uint32_t> stateIds;
    mutable std::unordered_set<std::string_view> inputs;

    uint32_t initialStatId;
    std::vector<uint32_t> acceptedStatIds;

    void addStateIfNotExist(uint32_t id) const {
        if (std::find(stateIds.begin(), stateIds.end(), id) == stateIds.end()) {
            stateIds.insert(std::lower_bound(stateIds.begin(), stateIds.end(), id), id);
        }
    }

    void gatherState(uint32_t id, std::unordered_set<uint32_t> &vis) const {
        addStateIfNotExist(id);
        vis.insert(id);
        State const & state = stateManager.getState(id);
        for (const auto &trans : state.getTransitions()) {
            inputs.insert(trans.input);
            if (vis.find(trans.nextStatId) == vis.end()) {
                gatherState(trans.nextStatId, vis);
            }
        }

    }

    void gatherNFAStatesAndInputs() const {
        inputs.clear();
        std::unordered_set<uint32_t> vis;
        gatherState(initialStatId, vis);
    }

public:

    /// \brief construct a new NFA instance with an newly-created initial state and no accepted states
    explicit NFA(StateManager &stateManager) : stateManager(stateManager) {
        initialStatId = stateManager.newState().getId();
        addStateIfNotExist(initialStatId);
    }

    State &getInitialState() const {
        return stateManager.getState(initialStatId);
    }

    void addAcceptedState(uint32_t id) {
        if (std::find(acceptedStatIds.begin(), acceptedStatIds.end(), id) == acceptedStatIds.end()) {
            addStateIfNotExist(id);
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
