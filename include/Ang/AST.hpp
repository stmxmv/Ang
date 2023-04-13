//
// Created by aojoie on 4/11/2023.
//

#ifndef ANG_AST_HPP
#define ANG_AST_HPP

#include <Basic/ASTContext.hpp>
#include <span>
#include <string>
#include <variant>
#include <vector>
#include <typeinfo>

namespace AN {

class AST;

class ASTVisitor {
public:
    virtual ~ASTVisitor()          = default;
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


class Term;
class ExprP;

class ExprBase : public AST {
public:
    virtual const char *getName() { return ""; }
    virtual bool isEmpty() { return false; }
};

class ValueAST : public ExprBase {

    std::string_view _value;

public:
    explicit ValueAST(std::string_view value) : _value(value) {}

    std::string_view getValue() const { return _value; }

    virtual void visit(ASTVisitor &visitor) override {
        visitor.onVisit(this);
        visitor.onVisitEnd(this);
    }
};

class AddOp : public ValueAST {
public:
    using ValueAST::ValueAST;
};

class MulOp : public ValueAST {
public:
    using ValueAST::ValueAST;
};

class ID : public ValueAST {
public:
    using ValueAST::ValueAST;
};

class Expr : public ExprBase {

    Term *_term;
    ExprP *_exprP;

public:
    Expr(Term *term, ExprP *exprP) : _term(term), _exprP(exprP) {}

    virtual void visit(ASTVisitor &visitor) override;

    virtual const char *getName() override { return "E"; }
};

class ExprP : public ExprBase {

    AddOp *_addOp;
    Term *_term;
    ExprP *_exprP;

public:
    ExprP() : _addOp(), _term(), _exprP() {}

    ExprP(AddOp *addOp, Term *term, ExprP *exprP) : _addOp(addOp), _term(term), _exprP(exprP) {}

    virtual void visit(ASTVisitor &visitor) override;

    virtual bool isEmpty() override {
        return _addOp == nullptr && _term == nullptr && _exprP == nullptr;
    }

    virtual const char *getName() override { return "E'"; }
};

class BracketExpr : public ExprBase {

    Expr *_expr;

public:
    explicit BracketExpr(Expr *expr) : _expr(expr) {}
    virtual void visit(ASTVisitor &visitor) override {
        visitor.onVisit(this);
        _expr->visit(visitor);
        visitor.onVisitEnd(this);
    }
};

class Factor : public ExprBase {

    AST *_value;/// can be ID * or BracketExpr *, not null

public:
    explicit Factor(AST *value) : _value(value) {}

    virtual void visit(ASTVisitor &visitor) override {
        visitor.onVisit(this);
        _value->visit(visitor);
        visitor.onVisitEnd(this);
    }

    virtual const char *getName() override { return "F"; }
};

class TermP : public ExprBase {

    MulOp *_mulOp;
    Factor *_factor;
    TermP *_termP;

public:

    TermP() : _mulOp(), _factor(), _termP() {}

    TermP(MulOp *mulOp, Factor *factor, TermP *termP) : _mulOp(mulOp), _factor(factor), _termP(termP) {}

    virtual void visit(ASTVisitor &visitor) override {
        visitor.onVisit(this);
        if (!isEmpty()) {
            _mulOp->visit(visitor);
            _factor->visit(visitor);
            _termP->visit(visitor);
        }
        visitor.onVisitEnd(this);
    }

    virtual bool isEmpty() override {
        return _mulOp == nullptr && _factor == nullptr && _termP == nullptr;
    }
    virtual const char *getName() override { return "T'"; }
};

class Term : public ExprBase {

    Factor *_factor;
    TermP *_termP;

public:
    Term(Factor *factor, TermP *termP) : _factor(factor), _termP(termP) {}

    virtual void visit(ASTVisitor &visitor) override {
        visitor.onVisit(this);
        _factor->visit(visitor);
        _termP->visit(visitor);
        visitor.onVisitEnd(this);
    }

    virtual const char *getName() override { return "T"; }
};



}// namespace AN

#endif//ANG_AST_HPP
