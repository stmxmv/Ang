//
// Created by aojoie on 3/21/2023.
//

#include "NFA/Parser.hpp"
#include "Basic/SourceFile.hpp"
#include "NFA/RegEx.hpp"

#include <fstream>

#define USAGE_STRING "Usage: NFA1 [input file path] [output file path]"

int main(int argc, const char * argv[]) {

    if (argc <= 2) {
        puts(USAGE_STRING);
        return -1;
    }

    AN::SourceFile file;
    AN::Error error;
    std::ofstream outFile(argv[2]);

    if (!file.init(argv[1], &error)) {
        printf("Open input file error: %s", error.getDescription().c_str());
        return -1;
    }

    if (!outFile.is_open()) {
        printf("Create output file error at path %s", argv[1]);
        return -1;
    }
    AN::NFA::StateManager stateManager;
    AN::NFA::Lexer lexer(file.getBuffer());
    AN::NFA::Parser parser(lexer, stateManager);

    std::unique_ptr<AN::NFA::NFA> nfa = parser.parse();

    if (nfa) {
        outFile << nfa->getPrettyString() << std::endl;
    } else {
        std::cout << "Error occurred when parsing" << std::endl;
    }

    outFile.close();


    return 0;
}