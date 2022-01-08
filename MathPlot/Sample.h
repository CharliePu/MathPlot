#pragma once
#include <array>
#include <boost/numeric/interval.hpp>

#include "Point.h"

struct Sample
{
	std::array<Point*, 4> points;
};
