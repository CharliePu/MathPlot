#pragma once
#include "Value.h"
class X :
    public Value
{
public:
    virtual double evaluate(double x, double y) override;
    virtual std::string getString() override;
};

