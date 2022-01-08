#pragma once
#include <optional>
#include <string>
#include "Interval.h"

class Expression
{
public:
	virtual std::unique_ptr<Expression> clone() = 0;
	virtual double evaluate(double x, double y) = 0;
	virtual Interval evaluateInterval(const Interval& xi, const Interval& yi) = 0;
	virtual std::string getString() = 0;
	virtual std::optional<double> getConstant() = 0;
};

