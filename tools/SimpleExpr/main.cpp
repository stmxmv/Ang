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

class GrammarVisitor : public AN::ASTVisitor {

    std::ostream &_ostream;

public:
    explicit GrammarVisitor(std::ostream &os) : _ostream(os) {}

    virtual void onVisit(AST *ast) override {
        if (auto *value = dynamic_cast<ValueAST *>(ast); value) {
            _ostream << value->getValue();
            return;
        }

        if (auto *exprBase = dynamic_cast<ExprBase *>(ast); exprBase) {
            _ostream << exprBase->getName();
            if (!exprBase->isEmpty()) {
                _ostream << '(';
            }
        }

    }

    virtual void onVisitEnd(AST *ast) override {
        if (auto *value = dynamic_cast<ValueAST *>(ast); value) {
            return;
        }

        if (auto *exprBase = dynamic_cast<ExprBase *>(ast); exprBase) {
            if (!exprBase->isEmpty()) {
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
    AN::Lexer lexer(file.getBuffer());
    AN::Parser parser(context, lexer);

    AST *ast = parser.parseSimpleExpr();

    GrammarVisitor visitor(outFile);
    ast->visit(visitor);

    outFile << endl;

    outFile.close();


    return 0;
}