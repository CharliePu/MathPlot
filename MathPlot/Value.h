#pragma once
#include <memory>

#include "Expression.h"
class Value :
    public Expression
{
};

std::unique_ptr<Value> createUnkown(char c);