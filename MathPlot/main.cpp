#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>

#include "Program.h"
#include "Renderer.h"
#include "Expression.h"
#include "Subtract.h"
#include "ExpressionParser.h"

std::vector<std::vector<double>> generateEquationData(const std::string &s, double xmin, double xmax, double ymin, double ymax, double step)
{
    size_t equalPos = s.find('=');
    ExpressionParser parser;

    auto l = parser.parse(s.substr(0, equalPos));
    auto r = parser.parse(s.substr(equalPos + 1));
    
    std::vector<std::vector<double>> data;

    if (l && r)
    {
        auto f = Subtract(std::move(l), std::move(r));
        for (double y = ymin; y <= ymax; y += step)
        {
            std::vector<double> row;
            for (double x = xmin; x <= xmax; x += step)
            {
                row.push_back(abs(f.evaluate(x, y)) < 0.2 ? 1.0 : 0.0);
            }
            data.push_back(row);
        }
    }
    return data;
}

int main()
{
    Program program(getProgram());
    Renderer renderer(program.screenWidth, program.screenHeight);



    renderer.updateData(generateEquationData("y = x * x * x", -5.0, 5.0, -2.0, 25.0, 0.05), true);

    while (!program.shouldClose())
    {
        renderer.draw();
    }

    return 0;
}