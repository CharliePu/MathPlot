#pragma once
#include "Operator.h"

class Subtract :
    public Operator
{
public:
    Subtract();
    Subtract(std::unique_ptr<Expression>&& l, std::unique_ptr<Expression>&& r);
    virtual std::unique_ptr<Expression> clone() override;
    virtual double evaluate(double x, double y) override;
    virtual Interval evaluateInterval(const Interval& xi, const Interval& yi) override;
    virtual std::string getString() override;
    virtual std::optional<double> getConstant() override;
private:
};