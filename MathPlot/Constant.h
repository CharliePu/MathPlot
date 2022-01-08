#pragma once
#include "Value.h"
class Constant :
    public Value
{
public:
    Constant(double c);
    virtual std::unique_ptr<Expression> clone() override;
    virtual double evaluate(double x, double y) override;
    virtual Interval evaluateInterval(const Interval& xi, const Interval& yi) override;
    virtual std::string getString() override;
    virtual std::optional<double> getConstant() override;
private:
    double c;
};

