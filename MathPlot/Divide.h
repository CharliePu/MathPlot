#pragma once
#include "Operator.h"
class Divide :
    public Operator
{
public:
    Divide();
    Divide(std::unique_ptr<Expression>&& l, std::unique_ptr<Expression>&& r);
    virtual std::unique_ptr<Expression> clone() override;
    virtual double evaluate(double x, double y) override;
    virtual std::string getString() override;
private:
};