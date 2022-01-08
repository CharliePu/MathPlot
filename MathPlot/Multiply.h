#pragma once
#include "Operator.h"
class Multiply :
    public Operator
{
public:
    Multiply();
    Multiply(std::unique_ptr<Expression>&& l, std::unique_ptr<Expression>&& r);
    virtual std::unique_ptr<Expression> clone() override;
    virtual double evaluate(double x, double y) override;
    virtual Interval evaluateInterval(const Interval& xi, const Interval& yi) override;
    virtual std::string getString() override;
private:
};