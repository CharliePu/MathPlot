#include "Program.h"
#include "Controller.h"

int main()
{
    Program program(getProgram());

    Controller controller(program);

    controller.start();

    return 0;
}