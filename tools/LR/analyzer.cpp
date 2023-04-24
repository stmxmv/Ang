//
// Created by aojoie on 3/1/2023.
//

#include "Basic/SourceFile.hpp"
#include "Grammar/Parser.hpp"

#include <LR/Parser.hpp>
#include <LR/Analyzer.hpp>
#include <iostream>
#include <fstream>

#define USAGE_STRING "Usage: LRTable [input file path] [input str] [output file path]"

int main(int argc, const char *argv[]) {

    if (argc <= 3) {
        puts(USAGE_STRING);
        return -1;
    }

    AN::SourceFile file;
    AN::Error error;
    std::ofstream outFile(argv[3]);

    if (!file.init(argv[1], &error)) {
        printf("Open input file error: %s", error.getDescription().c_str());
        return -1;
    }

    if (!outFile.is_open()) {
        printf("Create output file error at path %s", argv[1]);
        return -1;
    }

    AN::ASTContext context;
    AN::grammar::Lexer lexer(file.getBuffer());
    AN::grammar::Parser parser(context, lexer);

    AN::grammar::Grammar *grammar = parser.parse();

    AN::LR::Parser lrParser(grammar);

    AN::LR::Table lrTable =  lrParser.parseTable();

    std::ifstream inStrFile(argv[2]);
    std::string in;
    inStrFile >> in;

    AN::LR::Analyzer analyzer(lrTable, grammar);
    analyzer.analyze(in, outFile);

    outFile.close();

    return 0;
}