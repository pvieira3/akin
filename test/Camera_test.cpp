
#include "GraphicsWindow.h"

using namespace akin;

void keyboard(unsigned char key, int x, int y)
{

}

int main(int argc, char** argv)
{
    GraphicsWindow gui(argc, argv, "Camera_Test_Window", 800, 600,
                       verbosity::DEBUG);

    CheckGLError(gui.verb, "About to start running");
    gui.run();

    return 0;
}
