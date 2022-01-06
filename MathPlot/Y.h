#pragma once
#include "Value.h"
class Y :
    public Value
{
public:
    virtual std::unique_ptr<Expression> clone() override;
    virtual double evaluate(double x, double y) override;
    virtual boost::numeric::interval<double> evaluateInterval(const boost::numeric::interval<double>& xi, const boost::numeric::interval<double>& yi) override;
    virtual std::string getString() override;
};

