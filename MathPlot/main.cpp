#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <thread>
#include <atomic>
#include <mutex>
#include <condition_variable>

#include "Program.h"
#include "Controller.h"

int main()
{
    Program program(getProgram());

    Controller controller(program);

    controller.start();

    return 0;
}