#include "Statement.h"

#include <stdexcept>

#include "Subtract.h"

Statement::Statement(std::unique_ptr<Expression>&& lhs, Relation relation, std::unique_ptr<Expression>&& rhs):
	lhs(std::move(lhs)), relation(relation), rhs(std::move(rhs))
{
	if (!this->lhs || !this->rhs)
	{
		throw std::invalid_argument("lhs or rhs is nullptr");
	}
}

Statement::Statement(const Statement& other): lhs(other.lhs->clone()), rhs(other.rhs->clone()), relation(other.relation)
{
}

Statement& Statement::operator=(Statement other)
{
	std::swap(other.lhs, lhs);
	std::swap(other.rhs, rhs);
	std::swap(other.relation, relation);
	return *this;
}

std::unique_ptr<Expression> Statement::getExpression() const
{
	return std::make_unique<Subtract>(lhs->clone(), rhs->clone());
}

std::function<bool(double, double)> Statement::getComparator() const
{
	switch (relation)
	{
	case Relation::equal:
		return std::equal_to<double>();
	case Relation::less:
		return std::less<double>();
	case Relation::greater:
		return std::greater<double>();
	case Relation::lessEqual:
		return std::less_equal<double>();
	case Relation::greaterEqual:
		return std::greater_equal<double>();
	default:
		return [](double, double) {return true; };
	}
}

std::function<bool(Interval)> Statement::getIntervalCertaintyChecker() const
{
	switch (relation)
	{
	case Relation::equal:
	case Relation::less:
	case Relation::greater:
		return [](Interval i) {
			return (i.lower() == 0.0 && i.upper() == 0.0) || i.upper() < 0 || i.lower() > 0;
		};
	case Relation::lessEqual:
	case Relation::greaterEqual:
		return [](Interval i) {
			return i.upper() <= 0 || i.lower() >= 0;
		};
	default:
		return [](Interval i) { return false; };
	}
}

double Statement::evaluateInterval(Interval x, Interval y, double lineWidth)
{
	auto evaluatedInterval = getExpression()->evaluateInterval(x, y);
	if (relation == Relation::equal)
	{
		double r = 1 - std::abs(evaluatedInterval.lower() + evaluatedInterval.upper()) / lineWidth;
		return r > 0.9 ? r*r*r*r : 0.0;
	}
	else
	{
		double lbVal = getComparator()(evaluatedInterval.lower(), 0.0);
		double lbRat = abs(evaluatedInterval.lower()) / (abs(evaluatedInterval.lower()) + abs(evaluatedInterval.upper()));
		double ubVal = getComparator()(evaluatedInterval.upper(), 0.0);
		double ubRat = abs(evaluatedInterval.upper()) / (abs(evaluatedInterval.lower()) + abs(evaluatedInterval.upper()));
		return lbVal * lbRat + ubVal * ubRat;
	}
}

bool Statement::evaluate(double x, double y)
{
	return getComparator()(getExpression()->evaluate(x, y), 0.0);
}

std::string Statement::getString()
{
	std::string op;
	switch (relation)
	{
	case Relation::equal:
		op = "=";
		break;
	case Relation::less:
		op = "<";
		break;
	case Relation::greater:
		op = ">";
		break;
	case Relation::lessEqual:
		op = "<=";
		break;
	case Relation::greaterEqual:
		op = ">=";
		break;
	default:
		break;
	}
	return lhs->getString() + " " + op + " " + rhs->getString();
}
