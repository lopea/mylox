/**
 * ENTER DESCRIPTION HERE
 * @date 8/30/2022
 * @author Javier "Lopea" Sandoval (https://lopea.dev)
 * 
 */

#ifndef MYLOX_PARSER_H
#define MYLOX_PARSER_H


#include <utility>
#include <array>
#include <algorithm>
#include <exception>
#include "scanner.h"
#include "expr.h"

class ParserException : public std::exception
{
public:
    ParserException(TokenPtr, const std::string&): std::exception(
            ""
            ){}
};
class Parser
{
public:

    explicit Parser(std::vector<std::shared_ptr<Token>> tokens)
            : m_tokens(std::move(tokens)), m_current(0)
    {};

    ExprPtr Parse()
    {
        try
        {
            return Expression();
        } catch (const ParserException&)
        {
            return nullptr;
        }
    }
private:
    std::vector<std::shared_ptr<Token>> m_tokens;
    int m_current;

    [[nodiscard]] TokenPtr Previous() const
    {
        return m_tokens[m_current - 1];
    }

    TokenPtr Advance()
    {
        if (!EoF())
            m_current++;
        return Previous();
    }

    bool EoF()
    {
        return m_tokens[m_current]->m_type == TokenType::ENDOFFILE;
    }

    TokenPtr Peek()
    {
        return m_tokens[m_current];
    }

    bool Check(TokenType type)
    {
        return !EoF() && m_tokens[m_current]->m_type == type;
    }


    template<TokenType ... types>
    bool Match()
    {
        std::array<TokenType, sizeof...(types)> array{types...};

        if (std::ranges::any_of(array, [this](TokenType t)
        { return Check(t); }))
        {
            Advance();
            return true;
        }

        return false;
    }

    ExprPtr Expression()
    {
        return Equality();
    }

    ExprPtr Equality()
    {
        //get the next expression
        ExprPtr expr = Comparison();

        //get all the expressions
        while (Match<TokenType::EXC_EQUAL, TokenType::EQUAL_EQUAL>())
        {
            TokenPtr op = Previous();
            //get the next expression
            ExprPtr right = Comparison();

            //create the next expression
            expr = std::make_shared<BinaryExpr>(expr, op, right);
        }

        return expr;
    }

    ExprPtr Comparison()
    {
        ExprPtr expr = Term();

        while(Match<TokenType::GREATER, TokenType::GREATER_EQUAL,
                TokenType::LESS, TokenType::LESS_EQUAL>())
        {
            TokenPtr op = Previous();
            ExprPtr right = Term();
            expr = std::make_shared<BinaryExpr>(expr, op, right);
        }

        return expr;
    }

    ExprPtr Term()
    {
        ExprPtr expr = Factor();

        while(Match<TokenType::MINUS, TokenType::PLUS>())
        {
            TokenPtr op = Previous();
            ExprPtr right = Factor();

            expr = std::make_shared<BinaryExpr>(expr, op, right);
        }
        return expr;
    }
    ExprPtr Factor()
    {
        ExprPtr expr = Unary();

        while(Match<TokenType::SLASH, TokenType::STAR>())
        {
            TokenPtr op = Previous();
            ExprPtr right = Unary();

            expr = std::make_shared<BinaryExpr>(expr, op, right);
        }
        return expr;
    }


    ExprPtr Unary()
    {
        //TODO: Add inc/dec

        if(Match<TokenType::MINUS, TokenType::EXC>())
        {
            TokenPtr op = Previous();
            ExprPtr right = Unary();

            return std::make_shared<UnaryExpr>(op, right);
        }

        return Primary();
    }

    TokenPtr Consume(TokenType type, const char *message)
    {
        if(Check(type)) return Advance();

        throw ParserException(Peek(), message);
    }

    ExprPtr Primary()
    {
        if(Match<TokenType::FALSE>())
            return std::make_shared<LiteralExpr>(false);
        if(Match<TokenType::TRUE>())
            return std::make_shared<LiteralExpr>(true);
        if(Match<TokenType::NUL>())
            return std::make_shared<LiteralExpr>(std::any());

        if(Match<TokenType::NUMBER, TokenType::STRING>())
        {
            return std::make_shared<LiteralExpr>(Previous()->GetLiteral());
        }
        if(Match<TokenType::LPAREN>())
        {
            ExprPtr expr = Expression();
            Consume(TokenType::RPAREN, "Expected \')\' after expression");
            return std::make_shared<GroupingExpr>(expr);
        }
        throw ParserException(Peek(), "Expected Expression.");
    }




};


#endif //MYLOX_PARSER_H
