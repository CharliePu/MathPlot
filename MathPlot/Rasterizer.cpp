#include "Rasterizer.h"

void Rasterizer::rasterize(Plot plot, double step)
{
    double xmin = plot.getXMin();
    double xmax = plot.getXMax();
    double ymin = plot.getYMin();
    double ymax = plot.getYMax();

    Statement statement = plot.getStatement();

	data.clear();
    for (double y = ymin; y <= ymax; y += step)
    {
        std::vector<double> row;
        for (double x = xmin; x <= xmax; x += step)
        {
            row.push_back(statement.evaluate(x, y));
        }
        data.push_back(row);
    }
}

std::vector<std::vector<double>> Rasterizer::generateLines()
{
	return std::vector<std::vector<double>>();
}

std::vector<std::vector<double>> Rasterizer::generateRegions()
{
	return data;
}
