//
// Created by aojoie on 3/1/2023.
//

#include "Grammar/Parser.hpp"

#include <iostream>
#include <fstream>

const char *GrammarTypeToString(AN::grammar::GrammarType grammarType) {
    switch (grammarType) {
        case AN::grammar::GrammarTypeUnknown:
            return "Unknown Grammar";
        case AN::grammar::GrammarTypePSG:
            return "PSG";
        case AN::grammar::GrammarTypeCSG:
            return "CSG";
        case AN::grammar::GrammarTypeCFG:
            return "CFG";
        case AN::grammar::GrammarTypeRG_right:
            return "RG right";
        case AN::grammar::GrammarTypeRG_left:
            return "RG left";
        case AN::grammar::GrammarTypeRG:
            return "RG";
    }
}

const char *SymbolKindToString(AN::grammar::Symbol::SymbolKind type) {
    switch (type) {
        case AN::grammar::Symbol::UnknownSymbol:
            return "Unknown";
        case AN::grammar::Symbol::TSymbol:
            return "T";
        case AN::grammar::Symbol::VSymbol:
            return "V";
    }
}

#define USAGE_STRING "Usage: grammar [input file path] [output file path]"

int main(int argc, const char *argv[]) {

    if (argc <= 2) {
        puts(USAGE_STRING);
        return -1;
    }

    std::ifstream file(argv[1]);
    std::ofstream outFile(argv[2]);

    if (!file.is_open()) {
        printf("Open input file error at path %s", argv[1]);
        return -1;
    }

    if (!outFile.is_open()) {
        printf("Create output file error at path %s", argv[1]);
        return -1;
    }

    /// load file into a single buffer
    file.seekg(0, std::ios::end);
    size_t size = file.tellg();

    std::string buffer(size, 0);
    file.seekg(0);
    file.read(buffer.data(), size);

    AN::grammar::ASTContext context;
    AN::grammar::Lexer lexer(buffer.c_str());
    AN::grammar::Parser parser(context, lexer);

    AN::grammar::Grammar *grammar = parser.parse();

    outFile << grammar->getPrettyString();
    outFile.close();

    std::cout << "Grammar title is " << grammar->getGrammarTitle()->getName() <<
            " start symbol is " << grammar->getGrammarTitle()->getStartSymbol()->getVal() << '\n';

    printf("Each production below\n\n");

    for (const AN::grammar::Production *production : grammar->getProductions()) {
        std::cout << "type: " << GrammarTypeToString(production->getType()) << '\n';
        /// left symbol
        std::cout << "left symbol: ";
        for (const AN::grammar::Symbol *symbol : production->getLeftSymbols()) {
            std::cout << SymbolKindToString(symbol->getSymbolKind()) << ' ' << symbol->getVal() << ", ";
        }

        std::cout << "\nright symbol: ";

        /// right symbol

        for (const AN::grammar::Symbol *symbol : production->getRightSymbols()) {
            std::cout << SymbolKindToString(symbol->getSymbolKind()) << ' ' << symbol->getVal() << ", ";
        }

        std::cout << "\n\n";
    }


    printf("Grammar type is %s\n", GrammarTypeToString(grammar->getType()));

    return 0;
}