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

	[[nodiscard]] std::unique_ptr<Expression> getExpression() const;
	[[nodiscard]] std::function<bool(double, double)> getComparator() const;
	[[nodiscard]] std::function<bool(Interval)> getIntervalCertaintyChecker() const;
	[[nodiscard]] bool evaluate(double x, double y) const;
	[[nodiscard]] std::string getString() const;

private:
	Relation relation;
	std::unique_ptr<Expression> lhs, rhs;
};

