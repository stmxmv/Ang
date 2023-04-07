//
// Created by aojoie on 3/29/2023.
//

#include "NFA/RegEx.hpp"
#include <iostream>
#include <string>
#include <format>
#include <fstream>

#define USAGE_STRING "Usage: Thompson [input file path] [output file path]"

int main(int argc, const char *argv[]) {

    using std::cin, std::cout, std::endl;

    if (argc <= 2) {
        puts(USAGE_STRING);
        return -1;
    }

    std::ifstream fileIn(argv[1]);
    std::ofstream fileOut(argv[2]);

    if (!fileIn.is_open()) {
        printf("Read input file error at path %s", argv[1]);
        return -1;
    }

    if (!fileOut.is_open()) {
        printf("Create output file error at path %s", argv[1]);
        return -1;
    }

    std::string regex, line;
    while (std::getline(fileIn, line)) {
        if (!regex.empty()) {
            regex = std::format("({})|({})", regex, line);
        } else {
            regex = line;
        }
    }

    using namespace AN::NFA;
    StateManager stateManager;
    RegEx regEx(stateManager);

    std::unique_ptr<NFA> const nfa = regEx.parse(regex);
    nfa->getInitialState().setRawData("S");
    int cnt = 0;
    for (const auto &state : stateManager.getStates()) {
        if (*state->getRawData() == 0) {
            state->setRawData(std::string(1, 'A' + (cnt++)));
        }
    }

    fileOut << nfa->getPrettyString() << endl;
    fileOut.close();

    return 0;
}