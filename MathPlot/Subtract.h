#pragma once
#include "Operator.h"

class Subtract :
    public Operator
{
public:
    Subtract();
    Subtract(std::unique_ptr<Expression>&& l, std::unique_ptr<Expression>&& r);
    std::unique_ptr<Expression> clone() override;
    double evaluate(double x, double y) override;
    Interval evaluateInterval(const Interval& xi, const Interval& yi) override;
    std::string getString() override;
    std::optional<double> getConstant() override;
private:
};