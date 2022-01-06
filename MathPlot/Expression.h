#pragma once
#include <string>
#include <boost/numeric/interval.hpp>

class Expression
{
public:
	virtual std::unique_ptr<Expression> clone() = 0;
	virtual double evaluate(double x, double y) = 0;
	virtual boost::numeric::interval<double> evaluateInterval(const boost::numeric::interval<double>& xi, const boost::numeric::interval<double>& yi) = 0;
	virtual std::string getString() = 0;

};

