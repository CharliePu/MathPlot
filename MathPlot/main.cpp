#include "Program.h"
#include "Controller.h"

int main()
{
    Controller controller(getProgram());

    controller.start();

    return 0;
}