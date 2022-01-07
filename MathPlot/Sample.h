#pragma once
#include <array>
#include <boost/numeric/interval.hpp>

#include "Point.h"

using Interval = boost::numeric::interval<double>;

struct Sample
{
	std::array<Point*, 4> points;
};
