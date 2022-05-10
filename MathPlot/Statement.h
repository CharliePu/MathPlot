#pragma once
#include <memory>
#include <functional>

#include "Expression.h"

enum class Relation
{
	equal,
	less,
	greater,
	lessEqual,
	greaterEqual
};

class Statement
{
public:
	Statement(std::unique_ptr<Expression>&& lhs, Relation relation, std::unique_ptr<Expression>&& rhs);

	Statement(const Statement &other);
	Statement &operator=(Statement other);

	std::unique_ptr<Expression> getExpression() const;
	std::function<bool(double, double)> getComparator() const;
	std::function<bool(Interval)> getIntervalCertaintyChecker() const;
	double evaluateInterval(Interval x, Interval y, double lineWidth = 1.0);
	bool evaluate(double x, double y);
	std::string getString();

private:
	Relation relation;
	std::unique_ptr<Expression> lhs, rhs;
};

