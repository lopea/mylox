/**
 * ENTER DESCRIPTION HERE
 * @date 8/28/2022
 * @author Javier "Lopea" Sandoval (https://lopea.dev)
 * 
 */

#ifndef MYLOX_EXPR_H
#define MYLOX_EXPR_H

#include <any>
#include <memory>
#include <utility>
#include "scanner.h"
enum class ExprType
{
    Grouping,
    Unary,
    Binary,
    Literal
};

//Forward references
class Expr;
class GroupingExpr;
class LiteralExpr;
class UnaryExpr;
class BinaryExpr;

//forward declaration for the ptrs
using ExprPtr = std::shared_ptr<Expr>;
using GroupingExprPtr = std::shared_ptr<GroupingExpr>;
using LiteralExprPtr = std::shared_ptr<LiteralExpr>;
using UnaryExprPtr = std::shared_ptr<UnaryExpr>;
using BinaryExprPtr = std::shared_ptr<BinaryExpr>;



class ExprVisitor
{
public:
    virtual std::any VisitGrouping(const GroupingExprPtr&)
    {return {};}

    virtual std::any VisitUnary(const UnaryExprPtr&)
    {return {};}

    virtual std::any VisitBinary(const BinaryExprPtr&)
    {return {};}

    virtual std::any VisitLiteral(const LiteralExprPtr&)
    {return {};}
};

using ExprVisitorPtr = ExprVisitor*;

/**********************************************************************************************************************/
class Expr : public std::enable_shared_from_this<Expr>
{
public:
    virtual std::any Accept(ExprVisitorPtr visitor) = 0;

};

/**********************************************************************************************************************/
class GroupingExpr : public Expr
{
public:
    explicit GroupingExpr(ExprPtr expr) : m_expr(std::move(expr)) {}

    inline std::any Accept(ExprVisitorPtr visitor) override
    {
        return visitor->VisitGrouping(std::dynamic_pointer_cast<GroupingExpr>(shared_from_this()));
    }
    ExprPtr m_expr;
};

/**********************************************************************************************************************/
class UnaryExpr : public Expr
{
public:

    inline std::any Accept(ExprVisitorPtr visitor) override
    {
        return visitor->VisitUnary(std::dynamic_pointer_cast<UnaryExpr>(shared_from_this()));
    }
    UnaryExpr(TokenPtr op, ExprPtr expr)
    : m_expr(std::move(expr)),m_operator(std::move(op)) {}

    ExprPtr m_expr;
    TokenPtr m_operator;
};

/**********************************************************************************************************************/
class BinaryExpr : public Expr
{
public:
    BinaryExpr(ExprPtr left, TokenPtr op, ExprPtr right)
    : m_left(std::move(left)), m_expression(std::move(op)), m_right(std::move(right)) {}

    inline std::any Accept(ExprVisitorPtr visitor) override
    {
        return visitor->VisitBinary(std::dynamic_pointer_cast<BinaryExpr>(shared_from_this()));
    }


    ExprPtr m_left;
    TokenPtr m_expression;
    ExprPtr m_right;
};


/**********************************************************************************************************************/
class LiteralExpr : public Expr
{
public:
    explicit LiteralExpr(std::any literal) : m_value(std::move(literal)) {}
    inline std::any Accept(ExprVisitorPtr visitor) override
    {
        return visitor->VisitLiteral(std::dynamic_pointer_cast<LiteralExpr>(shared_from_this()));
    }

    std::any m_value;
};

#endif //MYLOX_EXPR_H
