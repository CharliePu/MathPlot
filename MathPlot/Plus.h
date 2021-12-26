#pragma once
#include "Operator.h"

class Plus :
    public Operator
{
public:
    Plus();
    Plus(std::unique_ptr<Expression>&& l, std::unique_ptr<Expression>&& r);
    virtual double evaluate(double x, double y) override;
    virtual int getPrecedence() override;
    virtual std::string getString() override;
private:
};

