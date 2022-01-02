#pragma once
#include "Operator.h"
class Absolute :
    public Operator
{
public:
    Absolute();
    Absolute(std::unique_ptr<Expression>&& e);
    virtual std::unique_ptr<Expression> clone() override;
    virtual double evaluate(double x, double y) override;
    virtual std::string getString() override;
};

