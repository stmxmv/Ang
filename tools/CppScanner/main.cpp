#include "Ang/Lexer.hpp"
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

    std::ifstream file(argv[1]);
    std::ofstream outFile(argv[2]);

    if (!file.is_open()) {
        printf("Open input file error at path %s", argv[1]);
        return -1;
    }

    if (!outFile.is_open()) {
        printf("Cannot create file at path %s", argv[2]);
        return -1;
    }

    /// load file into a single buffer
    file.seekg(0, std::ios::end);
    size_t size = file.tellg();

    std::string buffer(size, 0);
    file.seekg(0);
    file.read(buffer.data(), size);


    /// init lexer and process
    AN::Lexer lexer(buffer.c_str());

    AN::Token token;

    do {
        lexer.next(token);

        if (token.getKind() != AN::tok::eof) {
            auto outputName = (token.isKeyword() ? "Keyword "s + token.getName() : token.getName());
            outFile << '(' << outputName
                    << ", " << token.getRawData() << ')' << '\n';
//            printf("(%s, %s)\n", AN::tok::getTokenCategoryString(token.getKind()), std::string(token.getRawData()).c_str());
        }


    } while (token.getKind() != AN::tok::eof);



    return 0;
}
