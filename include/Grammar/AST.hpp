//
// Created by aojoie on 4/11/2023.
//

#ifndef ANG_AST_HPP
#define ANG_AST_HPP

#include "Basic/ASTContext.hpp"
#include <span>
#include <string>
#include <vector>
#include <variant>

namespace AN::grammar {

class AST;

class ASTVisitor {
public:
    virtual ~ASTVisitor() = default;
    virtual void onVisit(AST *ast) = 0;
    virtual void onVisitEnd(AST *ast) {}
};

class AST : public ASTAllocated {

public:


    virtual void visit(ASTVisitor &visitor) {}

    /// template version, which support lambda expression, not support onVisitEnd
    template<typename _Func>
    requires std::is_invocable_v<std::decay_t<_Func>, AST *>
    void visit(_Func &&func) {
        struct _tempASTVisitor : ASTVisitor {
            std::decay_t<_Func> &func;

            explicit _tempASTVisitor(std::decay_t<_Func> &f) : func(f) {}

            virtual void onVisit(AST *ast) final override {
                func(ast);
            }

        } tempAstVisitor(func);

        visit(static_cast<ASTVisitor &>(tempAstVisitor));
    }

};


class SymbolBase :  public AST {
    std::string _name;

public:

    explicit SymbolBase(std::string_view name) : _name(name) {}

    const char *getName() const { return _name.c_str(); }

};

class VSymbol : public SymbolBase {

    std::vector<AST *> _children;

public:

    explicit VSymbol(std::string_view name) : SymbolBase(name) {}

    VSymbol(std::string_view name, std::span<AST *> children)
        : SymbolBase(name), _children(children.begin(), children.end()) {}

    std::span<AST *const> getChildren() const { return {_children}; }

    void addChild(AST *child) {
        _children.push_back(child);
    }

    virtual void visit(ASTVisitor &visitor) override {
        visitor.onVisit(this);
        for (AST *child : _children) {
            child->visit(visitor);
        }
        visitor.onVisitEnd(this);
    }
};

class TSymbol : public SymbolBase {

public:

    using SymbolBase::SymbolBase;

    virtual void visit(ASTVisitor &visitor) override {
        visitor.onVisit(this);
        visitor.onVisitEnd(this);
    }

};


}// namespace AN::Grammar

#endif//ANG_AST_HPP
