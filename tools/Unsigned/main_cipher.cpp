//
// Created by aojoie on 3/21/2023.
//

#include "Unsigned/Lexer.hpp"
#include "Basic/SourceFile.hpp"

#include <fstream>
#include <unordered_map>

const char *tokenToString(AN::Unsigned::tok::TokenKind kind) {
    switch (kind) {
        case AN::Unsigned::tok::unknown:
            return "Unknown";
        case AN::Unsigned::tok::eof:
            return "eof";
        case AN::Unsigned::tok::number:
            return "数字";
        case AN::Unsigned::tok::other:
            return "其他";
        case AN::Unsigned::tok::error:
            return "异常";
        case AN::Unsigned::tok::num:
            return "";
    }
}

#define USAGE_STRING "Usage: Unsigned [input file path] [output file path]"

int main(int argc, const char * argv[]) {

    if (argc <= 2) {
        puts(USAGE_STRING);
        return -1;
    }

    AN::SourceFile file;
    AN::Error error;
    std::ofstream intermediateFile("temp.txt");
    std::ofstream outFile(argv[2]);

    if (!outFile.is_open()) {
        printf("Create output file error at path %s", argv[1]);
        return -1;
    }

    if (!intermediateFile.is_open()) {
        printf("Create intermediate output File error");
        return -1;
    }


    static const std::unordered_map<char, char> cipherMap = {
            { 'A', '0' }, { 'B', '1' }, { 'C', '2' },
            { 'D', '3' }, { 'E', '4' }, { 'F', '5' },
            { 'G', '6' }, { 'H', '7' }, { 'I', '8' },
            { 'J', '9' }, { 'K', '.' }, { 'L', 'E' },
            { 'M', '+' }, { 'N', '-' }
    };

    if (!file.init(argv[1], &error)) {
        printf("Open input file error: %s", error.getDescription().c_str());
        return -1;
    }

    for (const char *cur = file.getBuffer(); *cur; ++cur) {
        if (auto iter = cipherMap.find(*cur); iter != cipherMap.end()) {
            intermediateFile << iter->second;
        } else {
            intermediateFile << *cur;
        }
    }

    intermediateFile.close();
    file.close();

    if (!file.open("temp.txt", &error)) {
        printf("Open intermediate file error: %s", error.getDescription().c_str());
        return -1;
    }


    AN::Unsigned::Lexer lexer(file.getBuffer());

    AN::Unsigned::Token token;

    do {

        lexer.next(token);

        if (token.is(AN::Unsigned::tok::number)) {
            outFile << "(" << tokenToString(token.getKind()) << ", " << token.getRawData() << ")\n";

        }


    } while (token.isNot(AN::Unsigned::tok::eof));

    outFile.close();


    return 0;
}