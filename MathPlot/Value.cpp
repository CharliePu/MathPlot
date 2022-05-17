#include "Value.h"

#include "X.h"
#include "Y.h"

std::unique_ptr<Value> createUnkown(char c)
{
	if (c == 'x')
	{
		return std::make_unique<X>();
	}
	if (c == 'y')
	{
		return std::make_unique<Y>();
	}
	return nullptr;
}
