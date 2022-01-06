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
    virtual boost::numeric::interval<double> evaluateInterval(const boost::numeric::interval<double>& xi, const boost::numeric::interval<double>& yi) override;
    virtual std::string getString() override;
private:
};