#pragma once
#include "Value.h"
class X :
    public Value
{
public:
    virtual std::unique_ptr<Expression> clone() override;
    virtual double evaluate(double x, double y) override;
    virtual std::string getString() override;
};

