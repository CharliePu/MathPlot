#include <iostream>
#include <unordered_set>

#include "Constant.h"
#include "Plus.h"
#include "X.h"
#include "Y.h"
#include "Multiply.h"

int main()
{
	Constant c1(3.0);
	X x;
	Y y;
	Multiply exp1(std::make_unique<Constant>(c1), std::make_unique<X>(x));
	// c1 = 3, x
	// c1 * x
	std::cout << exp1.evaluate(2.0, 2.0) << std::endl;
	return 0;
}