//
// Created by aojoie on 3/29/2023.
//

#include "NFA/RegEx.hpp"
#include <iostream>
#include <string>
#include <format>
#include <fstream>
#include <sstream>

#define USAGE_STRING "Usage: Thompson [input file path] [output file path]"

int main(int argc, const char *argv[]) {

    using std::cin, std::cout, std::endl;

    if (argc <= 2) {
        puts(USAGE_STRING);
        return -1;
    }

    std::ifstream rex_file(argv[1]), str_file(argv[2]);

    if (!rex_file.is_open()) {
        printf("Read rex_file file error at path %s", argv[1]);
        return -1;
    }

    if (!str_file.is_open()) {
        printf("Create str_file error at path %s", argv[2]);
        return -1;
    }

    std::vector<std::string> regexes, classes, strings;
    std::string line;

    while (std::getline(rex_file, line)) {
        std::stringstream ss(line);
        std::string reg, cls;
        ss >> reg >> cls;
        regexes.push_back(reg);
        classes.push_back(cls);
    }

    while (std::getline(str_file, line)) {
        strings.push_back(line);
    }

    using namespace AN::NFA;
    StateManager stateManager;
    RegEx regEx(stateManager);

    std::vector<std::unique_ptr<NFA>> nfas;
    for (const auto &reg : regexes) {
        nfas.push_back(regEx.parse(reg));
        if (nfas.back() == nullptr) {
            cout << "Error when parsing " << reg << endl;
            return 1;
        }
    }

//    int cnt = 0;
//    for (const auto &state : stateManager.getStates()) {
//        if (state->getRawData()) {
//            state->setRawData(std::string(1, 'A' + (cnt++)));
//        }
//    }
//    cout << nfas.back()->getPrettyString() << endl;

    for (const auto &str : strings) {
        cout << str << " class: " ;
        for (int i = 0; i < nfas.size(); ++i) {
            if (nfas[i]->match(str)) {
                cout << classes[i] << ' ';
            }
        }
        cout << '\n';
    }

    cout.flush();

    return 0;
}