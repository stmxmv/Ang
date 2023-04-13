//
// Created by aojoie on 3/1/2023.
//

#include "Basic/SourceFile.hpp"
#include "Grammar/Parser.hpp"

#include <fstream>
#include <iostream>


#define USAGE_STRING "Usage: grammarAST [input file path] [output file path]"

using namespace AN::grammar;
using std::cout, std::endl;

class GrammarVisitor : public ASTVisitor {

    std::string _leafString;
    std::ostream &_ostream;

public:
    explicit GrammarVisitor(std::ostream &os) : _ostream(os) {}

    const char *getLeafString() const { return _leafString.c_str(); }

    virtual void onVisit(AST *ast) override {
        if (auto *vSymbol = dynamic_cast<VSymbol *>(ast); vSymbol) {

            _ostream << vSymbol->getName();
            if (!vSymbol->getChildren().empty()) {
                _ostream << '(';
            } else {
                _leafString.append(vSymbol->getName());
            }

        } else if (auto *tSymbol = dynamic_cast<TSymbol *>(ast); tSymbol) {

            _ostream << tSymbol->getName();
            _leafString.append(tSymbol->getName());
        }
    }

    virtual void onVisitEnd(AST *ast) override {
        if (auto *vSymbol = dynamic_cast<VSymbol *>(ast); vSymbol) {

            if (!vSymbol->getChildren().empty()) {
                _ostream << ')';
            }
        }
    }
};

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
    AN::grammar::Lexer lexer(file.getBuffer());
    AN::grammar::Parser parser(context, lexer);

    AST *ast = parser.parseAST();

    GrammarVisitor visitor(outFile);
    ast->visit(visitor);

    outFile << endl;
    outFile << visitor.getLeafString() << endl;

    outFile.close();


    return 0;
}