#pragma once
#include <vector>
#include <memory>
#include <string>

#include "Expression.h"

class Operator :
    public Expression
{
public:
    Operator();
    Operator(std::vector<std::unique_ptr<Expression>>&& exprs);
    Operator(std::unique_ptr<Expression>&& expr);
    Operator(std::unique_ptr<Expression>&& l, std::unique_ptr<Expression>&& r);

    std::vector<std::unique_ptr<Expression>> cloneExpressions();

    std::unique_ptr<Expression>& getFirstExpression();
    std::unique_ptr<Expression>& getSecondExpression();

    void setFirstExpression(std::unique_ptr<Expression>&& expr);
    void setSecondExpression(std::unique_ptr<Expression>&& expr);

private:
    std::vector<std::unique_ptr<Expression>> exprs;
};


std::unique_ptr<Operator> createOperator(char c);