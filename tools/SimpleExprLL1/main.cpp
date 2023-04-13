//
// Created by aojoie on 3/1/2023.
//

#include "Basic/SourceFile.hpp"
#include <Ang/Parser.hpp>

#include <fstream>
#include <iostream>


#define USAGE_STRING "Usage: SimpleAST [input file path] [output file path]"

using std::cout, std::endl;
using namespace AN;

int main(int argc, const char *argv[]) {

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


    AN::ASTContext context;
    AN::Lexer lexer(file.getBuffer());
    AN::Parser parser(context, lexer);

    LL1Map map;
    map["E"]["i"].emplace_back("T");
    map["E"]["i"].emplace_back("E'");
    map["E"]["("].emplace_back("T");
    map["E"]["("].emplace_back("E'");

    map["E'"]["+"].emplace_back("+");
    map["E'"]["+"].emplace_back("T");
    map["E'"]["+"].emplace_back("E'");

    map["E'"][")"]; // epsilon
    map["E'"]["#"]; // epsilon

    map["T"]["i"].emplace_back("F");
    map["T"]["i"].emplace_back("T'");

    map["T"]["("].emplace_back("F");
    map["T"]["("].emplace_back("T'");

    map["T'"]["+"]; // epsilon
    map["T'"]["*"].emplace_back("*");
    map["T'"]["*"].emplace_back("F");
    map["T'"]["*"].emplace_back("T'");
    map["T'"][")"]; // epsilon
    map["T'"]["#"]; // epsilon

    map["F"]["i"].emplace_back("i");
    map["F"]["("].emplace_back("(");
    map["F"]["("].emplace_back("E");
    map["F"]["("].emplace_back(")");

    std::set<std::string> tSymbols{ "i", "+", "*", "(", ")", "#" };
    outFile << parser.parseSimpleExpr(map, tSymbols, "E");
    outFile << endl;

    outFile.close();


    return 0;
}