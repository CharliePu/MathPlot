#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>

#include "Program.h"
#include "Renderer.h"

int main()
{
    Program program(getProgram());
    Renderer renderer;

    while (!program.shouldClose())
    {
        renderer.draw();
    }

    return 0;
}