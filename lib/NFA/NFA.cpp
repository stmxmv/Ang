//
// Created by aojoie on 3/29/2023.
//

#include "NFA/NFA.hpp"

#include <algorithm>


namespace AN::NFA {


bool NFA::match(std::string_view str) {

    std::unordered_set<uint32_t> stateSets{ initialStatId };
    std::vector<uint32_t> currentStates;

    auto extend_epsilon_state = [&] {
        for (auto iter = stateSets.begin(); iter != stateSets.end();) {
            State &state = stateManager.getState(*iter);
            std::vector<uint32_t> const epsilon_nexts = state.getNextStateId("ε");
            bool change = false;
            for (uint32_t const id : epsilon_nexts) {
                if (!stateSets.contains(id)) {
                    change = true;
                    stateSets.insert(id);
                }
            }
            if (change) {
                iter = stateSets.begin();
            } else {
                ++iter;
            }
        }
    };

    for (char const ch : str) {

        if (stateSets.empty()) {
            return false;
        }

        extend_epsilon_state();

        currentStates.assign(stateSets.begin(), stateSets.end());
        stateSets.clear();

        for (uint32_t const id : currentStates) {
            State &state = stateManager.getState(id);
            std::vector<uint32_t> const nexts = state.getNextStateId(std::string(1, ch));

            stateSets.insert(nexts.begin(), nexts.end());
        }
    }

    extend_epsilon_state();

    return std::ranges::any_of(stateSets, [this](uint32_t id) { return isAcceptedState(id); });
}

namespace {

void addStateIfNotExist(uint32_t id, std::vector<uint32_t> &stateIds) {
    if (std::find(stateIds.begin(), stateIds.end(), id) == stateIds.end()) {
        stateIds.insert(std::lower_bound(stateIds.begin(), stateIds.end(), id), id);
    }
}

void gatherState(uint32_t id,
                 std::vector<uint32_t> &stateIds,
                 std::unordered_set<std::string_view> &inputs,
                 std::unordered_set<uint32_t> &vis,
                 StateManager &stateManager) {
    addStateIfNotExist(id, stateIds);
    vis.insert(id);
    State const & state = stateManager.getState(id);
    for (const auto &trans : state.getTransitions()) {
        inputs.insert(trans.input);
        if (vis.find(trans.nextStatId) == vis.end()) {
            gatherState(trans.nextStatId, stateIds, inputs, vis, stateManager);
        }
    }

}

void gatherNFAStatesAndInputs(uint32_t initialStatId,
                              std::vector<uint32_t> &stateIds,
                              std::unordered_set<std::string_view> &inputs,
                              StateManager &stateManager) {
    inputs.clear();
    std::unordered_set<uint32_t> vis;
    gatherState(initialStatId, stateIds, inputs, vis, stateManager);
}


}

std::string NFA::getPrettyString() const {
    std::vector<uint32_t> stateIds;
    std::unordered_set<std::string_view> inputs;

    gatherNFAStatesAndInputs(initialStatId, stateIds, inputs, stateManager);

    std::string result;
    result.append("K = { ");
    for (int i = 0; i < stateIds.size(); ++i) {
        if (i) {
            result.append(", ");
        }
        result.append(stateManager.getState(stateIds[i]).getRawData());
    }

    result.append(" };\nΣ = { ");

    for (auto iter = inputs.begin(); iter != inputs.end(); ++iter) {
        if (iter != inputs.begin()) {
            result.append(", ");
        }
        result.append(*iter);
    }

    result.append(" };\n");

    // all transitions
    bool first = true;
    int cnt = 0;
    for (uint32_t const id: stateIds) {
        const auto &stat = stateManager.getState(id);
        for (const Transition &transition : stat.getTransitions()) {
            if (first) {
                first = false;
            } else {
                result.append(", ");
            }

            if (++cnt % 9 == 0) {
                result.append("\n");
            }
            result.append("f(");
            result.append(stat.getRawData());
            result.append(", ");
            result.append(transition.input);
            result.append(") = ");
            result.append(stateManager.getState(transition.nextStatId).getRawData());

        }
    }

    result.append(";\n");
    result.append(stateManager.getState(initialStatId).getRawData());
    result.append("; ");

    // accepted state
    result.append("\nZ = { ");
    for (int i = 0; i < acceptedStatIds.size(); ++i) {
        if (i) {
            result.append(", ");
        }
        result.append(stateManager.getState(acceptedStatIds[i]).getRawData());
    }
    result.append(" }");
    return result;
}

}