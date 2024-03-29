#include "Operator.h"

#include "Plus.h"
#include "Subtract.h"
#include "Divide.h"
#include "Multiply.h"
#include "Exponent.h"

Operator::Operator()
= default;

Operator::Operator(std::vector<std::unique_ptr<Expression>>&& exprs): exprs(std::move(exprs))
{
}

Operator::Operator(std::unique_ptr<Expression>&& expr): exprs()
{
    exprs.emplace_back(std::move(expr));
}

Operator::Operator(std::unique_ptr<Expression>&& l, std::unique_ptr<Expression>&& r): exprs()
{
    exprs.emplace_back(std::move(l));
    exprs.emplace_back(std::move(r));
}

std::vector<std::unique_ptr<Expression>> Operator::cloneExpressions() const
{
    std::vector<std::unique_ptr<Expression>> newExprs;
    for (const auto& expr : exprs)
    {
        newExprs.emplace_back(expr->clone());
    }

    return std::move(newExprs);
}

std::unique_ptr<Expression>& Operator::getFirstExpression()
{
    return exprs.at(0);
}

std::unique_ptr<Expression>& Operator::getSecondExpression()
{
    return exprs.at(1);
}

void Operator::setFirstExpression(std::unique_ptr<Expression>&& expr)
{
    if (exprs.size() < 1)
    {
        exprs.resize(1);
    }
    exprs[0] = std::move(expr);
}

void Operator::setSecondExpression(std::unique_ptr<Expression>&& expr)
{
    if (exprs.size() < 2)
    {
        exprs.resize(2);
    }
    exprs[1] = std::move(expr);
}

std::unique_ptr<Operator> createOperator(char c)
{
    if (c == '+')
    {
        return std::make_unique<Plus>();
    }
    if (c == '-')
    {
        return std::make_unique<Subtract>();
    }
    if (c == '*')
    {
        return std::make_unique<Multiply>();
    }
    if (c == '/')
    {
        return std::make_unique<Divide>();
    }
    if (c == '^')
    {
        return std::make_unique<Exponent>();
    }

    return nullptr;
}
