#pragma once
#include "Operator.h"
class Multiply :
    public Operator
{
public:
    Multiply();
    Multiply(std::unique_ptr<Expression>&& l, std::unique_ptr<Expression>&& r);
    std::unique_ptr<Expression> clone() override;
    double evaluate(double x, double y) override;
    Interval evaluateInterval(const Interval& xi, const Interval& yi) override;
    std::string getString() override;
    std::optional<double> getConstant() override;
private:
};