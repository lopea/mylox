/**
 * ENTER DESCRIPTION HERE
 * @date 2022-10-03
 * @author Javier "Lopea" Sandoval (https://lopea.dev)
 * 
 */

#ifndef MYLOX_PRINTER_H
#define MYLOX_PRINTER_H
#include "expr.h"
#include <string>
#include <sstream>


class Printer : public ExprVisitor
{
public:
    std::string Print(ExprPtr expr) { return std::any_cast<std::string>(expr->Accept(this));}

    std::any VisitBinary(const BinaryExprPtr & expr) override
    {
        return parenthesize(expr->m_expression->m_lexeme, {expr->m_left, expr->m_right});
    }

    std::any VisitGrouping(const GroupingExprPtr & expr) override
    {
        return parenthesize("group", {expr->m_expr});
    }

    std::any VisitLiteral(const LiteralExprPtr &expr) override
    {
        if(!expr->m_value.has_value())
            return std::string("null");

        if(expr->m_value.type() == typeid(std::string))
            return expr->m_value;
        if(expr->m_value.type() == typeid(int))
            return std::to_string(std::any_cast<int>(expr->m_value));
        if(expr->m_value.type() == typeid(float))
            return std::to_string(std::any_cast<float>(expr->m_value));
        return nullptr;
    }

    std::any VisitUnary(const UnaryExprPtr &expr) override
    {
        return parenthesize(expr->m_operator->m_lexeme, {expr->m_expr});
    }

private:
    std::string parenthesize(const std::string& name, const std::vector<ExprPtr>& exprs)
    {
        std::stringstream stream;
        stream << '(' << name;

        for(const auto& expr : exprs)
        {
            auto ret = expr->Accept(this);
            stream << " " << std::any_cast<std::string>(ret);
        }
        stream << ')';

        return stream.str();
    }
};


#endif //MYLOX_PRINTER_H
