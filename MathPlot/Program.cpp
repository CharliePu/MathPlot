#include "Program.h"

std::function<void(int, int)> sizeChangeFunction;

void framebufferSizeCallback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
    sizeChangeFunction(width, height);
}

Program& getProgram()
{
    static Program program{};
    return program;
}

Program::Program()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window = glfwCreateWindow(screenWidth, screenHeight, title, NULL, NULL);
    if (!window)
    {
        glfwTerminate();
    }

    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);
    
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        
    }
}

Program::~Program()
{
    glfwTerminate();
}

bool Program::keyPressed(char c)
{
    if (glfwGetKey(window, c) == GLFW_PRESS && !keyState[c])
    {
        keyState[c] = true;
        return true;
    }
    else
    {
        keyState[c] = false;
        return false;
    }
}

bool Program::mouseDragged()
{
    prevMouseX = mouseX;
    prevMouseY = mouseY;
    glfwGetCursorPos(window, &mouseX, &mouseY);
    return glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS && mouseX != prevMouseX && mouseY != prevMouseY;
}

double Program::getMouseDeltaX()
{
    return mouseX - prevMouseX;
}

double Program::getMouseDeltaY()
{
    return mouseY - prevMouseY;
}

void Program::setOnWindowSizeChange(const std::function<void(int, int)>& f)
{
    sizeChangeFunction = f;
}

bool Program::shouldClose()
{
    glfwSwapBuffers(window);
    glfwPollEvents();

    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    glClearColor(0.2f, 0.5f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    return glfwWindowShouldClose(window);
}

