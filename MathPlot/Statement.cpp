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

std::unique_ptr<Expression> Statement::getExpression()
{
	return std::make_unique<Subtract>(lhs->clone(), rhs->clone());
}

bool Statement::evaluate(double x, double y)
{
	auto l = lhs->evaluate(x, y);
	auto r = rhs->evaluate(x, y);
	switch (relation)
	{
	case Relation::equal:
		return l == r;
	case Relation::less:
		return l < r;
	case Relation::greater:
		return l > r;
	case Relation::lessEqual:
		return l <= r;
	case Relation::greaterEqual:
		return l >= r;
	default:
		return false;
	}
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
