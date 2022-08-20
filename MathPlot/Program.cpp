#include "Program.h"

#include <iostream>
#include <ostream>
#include <queue>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

std::function<void(int, int)> sizeChangeFunction;

bool scrolled;
double scrollY;

enum class KeyState
{
    PRESS = GLFW_PRESS,
    REPEAT = GLFW_REPEAT,
    RELEASE = GLFW_RELEASE
};

std::unordered_map<int, KeyState> keyState;

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    keyState[key] = static_cast<KeyState>(action);
}

void framebufferSizeCallback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
    sizeChangeFunction(width, height);
}

void scrollCallback(GLFWwindow* window, double xoffset, double yoffset)
{
    scrolled = true;
    scrollY = yoffset;
}

std::queue<char> textInputQueue;

void characterCallback(GLFWwindow* window, unsigned int codepoint)
{
    textInputQueue.push(static_cast<char>(codepoint));
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

    window = glfwCreateWindow(screenWidth, screenHeight, title, nullptr, nullptr);
    if (!window)
    {
        glfwTerminate();
    }

    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);
    glfwSwapInterval(1);
    
    if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress)))
    {
        
    }

    glfwSetScrollCallback(window, scrollCallback);

    glfwSetKeyCallback(window, keyCallback);

    glfwSetCharCallback(window, characterCallback);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glEnable(GL_LINE_SMOOTH);
    glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
}

Program::~Program()
{
    glfwTerminate();
}

bool Program::keyPressing(char c)
{
    return keyPressing(static_cast<int>(c));
}

bool Program::keyPressed(char c)
{
    return keyPressed(static_cast<int>(c));
}

bool Program::enterPressed()
{
    return keyPressed(GLFW_KEY_ENTER);
}

bool Program::escapePressed()
{
    return keyPressed(GLFW_KEY_ESCAPE);
}

bool Program::backSpacePressed()
{
    return keyPressed(GLFW_KEY_BACKSPACE);
}

bool Program::mouseClicked()
{
    return glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS;
}

bool Program::mouseDragged()
{
    prevMouseX = mouseX;
    prevMouseY = mouseY;
    glfwGetCursorPos(window, &mouseX, &mouseY);

    // [0, width], [0, height] => [-1.0, 1.0], [-1.0, 1.0]
    mouseX = 2 * mouseX / getWidth() - 1.0;
    mouseY = 2 * mouseY / getHeight() - 1.0;

    // y of mouse position is inverted
    mouseY *= -1;

    return glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS && mouseX != prevMouseX && mouseY != prevMouseY;
}

double Program::getMouseX() const
{
    return mouseX;
}

double Program::getMouseY() const
{
    return mouseY;
}

double Program::getMouseDeltaX() const
{
    return mouseX - prevMouseX;
}

double Program::getMouseDeltaY() const
{
    return mouseY - prevMouseY;
}

bool Program::hasTextInput()
{
    return !textInputQueue.empty();
}

char Program::getTextInput()
{
    const char c{ textInputQueue.front() };
    textInputQueue.pop();
    return c;
}

bool Program::mouseScrolled()
{
    return scrolled;
}

double Program::getMouseScroll()
{
    scrolled = false;
    return scrollY;
}

int Program::getWidth() const
{
    int r;
    glfwGetFramebufferSize(window, &r, nullptr);
    return r;
}

int Program::getHeight() const
{
    int r;
    glfwGetFramebufferSize(window, nullptr, &r);
    return r;
}

void Program::setOnWindowSizeChange(const std::function<void(int, int)>& f)
{
    sizeChangeFunction = f;
}

bool Program::shouldClose() const
{
    glfwSwapBuffers(window);
    glfwPollEvents();

    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    return glfwWindowShouldClose(window);
}

bool Program::keyPressing(int c)
{
    return keyState.contains(c) && (keyState[c] == KeyState::PRESS || keyState[c] == KeyState::REPEAT);
}

bool Program::keyPressed(int c)
{
    if (!keyState.contains(c) || keyState[c] != KeyState::PRESS)
    {
        return false;
    }

    keyState[c] = KeyState::REPEAT;
    return true;
}

