#include "Ang/Lexer.hpp"
#include "Basic/SourceFile.hpp"
#include <cstdio>
#include <fstream>
#include <sstream>

#define USAGE_STRING "Usage: Ang [input file path] [output file path]"

int main(int argc, const char *argv[]) {

    using namespace std::literals;

    if (argc <= 2) {
        puts(USAGE_STRING);
        return -1;
    }

    // init SourceFile
    AN::SourceFile file;
    AN::Error error;

    if (!file.init(argv[1], &error)) {
        printf("open input file error: %s", error.getDescription().c_str());
        return -1;
    }

    std::ofstream outFile(argv[2]);


    if (!outFile.is_open()) {
        printf("Cannot create file at path %s", argv[2]);
        return -1;
    }



    /// init lexer and process
    AN::Lexer lexer(file.getBuffer());

    AN::Token token;

    for (;;) {
        lexer.next(token);

        if (token.getKind() != AN::tok::eof) {
            auto outputName = (token.isKeyword() ? "Keyword "s + token.getName() : token.getName());
            outFile << '(' << outputName << ", " << token.getRawData() << ')' << '\n';
            //            printf("(%s, %s)\n", AN::tok::getTokenCategoryString(token.getKind()), std::string(token.getRawData()).c_str());
        } else {
            break;
        }
    }


    return 0;
}
