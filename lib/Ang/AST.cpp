//
// Created by aojoie on 4/11/2023.
//

#include "Ang/AST.hpp"

void AN::Expr::visit(AN::ASTVisitor &visitor) {
    visitor.onVisit(this);
    _term->visit(visitor);
    _exprP->visit(visitor);
    visitor.onVisitEnd(this);
}

void AN::ExprP::visit(AN::ASTVisitor &visitor) {
    visitor.onVisit(this);
    if (!isEmpty()) {
        _addOp->visit(visitor);
        _term->visit(visitor);
        _exprP->visit(visitor);
    }
    visitor.onVisitEnd(this);
}