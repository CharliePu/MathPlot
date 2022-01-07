#include "SampleTree.h"

bool SampleTree::check(double x, double y)
{
	return check(&root, x, y);
}

bool SampleTree::check(std::unique_ptr<SampleTreeNode>* node, double x, double y)
{
	if (!node)
	{
		throw std::logic_error("SampleTree:check definite state not found");
	}

	if ((*node)->determinedState.has_value())
	{
		return (*node)->determinedState.value();
	}
	else
	{
		const double midX = ((*node)->xi.upper() + (*node)->xi.lower()) / 2.0;
		const double midY = ((*node)->yi.upper() + (*node)->yi.lower()) / 2.0;

		if (!(*node)->xSplit)
		{
			if (y < midY)
			{
				return check(&(*node)->nodes[0], x, y);
			}
			else
			{
				return check(&(*node)->nodes[1], x, y);
			}
		}

		if (!(*node)->ySplit)
		{
			if (x < midX)
			{
				return check(&(*node)->nodes[0], x, y);
			}
			else
			{
				return check(&(*node)->nodes[1], x, y);
			}
		}

		if (x < midX)
		{
			if (y < midY)
			{
				return check(&(*node)->nodes[0], x, y);
			}
			else
			{
				return check(&(*node)->nodes[1], x, y);
			}
		}
		else
		{
			if (y < midY)
			{
				return check(&(*node)->nodes[2], x, y);
			}
			else
			{
				return check(&(*node)->nodes[3], x, y);
			}
		}
	}
}
