#ifndef GRAPHICSWINDOW_H
#define GRAPHICSWINDOW_H

#include "IncludeGL.h"

#include "GraphicsBuffer.h"
#include "GraphicsCamera.h"

namespace akin {

class GraphicsWindow
{
public:
    
    GraphicsWindow(int argc, char* argv[],
                   std::string name = "window", int width=800, int height=600,
                   verbosity::verbosity_level_t report_level = verbosity::LOG);

    bool checkInstance();
    

    static void Resize(int new_width, int new_height);
    static void Render();

    static int get_current_width();
    static int get_current_height();
    static int get_window_handle();
    static void run();
    static std::string get_window_name();
    static void static_keyboard(unsigned char key, int x, int y);
    static void simple_move_camera(float x, float dy, float dz);
    static void static_mouse(int button, int state, int x, int y);
    static void static_motion(int x, int y);
    
    
    verbosity verb;
    
protected:

    virtual void _keyboard(unsigned char key, int x, int y);
    virtual void _mouse(int button, int state, int x, int y);
    virtual void _motion(int x, int y);
    /**
     * \brief Converts a mouse click to a camera motion mode.
     * This includes pressing Shift, Ctrl or Alt key while pressing
     * the left mouse button.
     */
    GraphicsCamera::MouseMode mouseToCamera(int button);
    GraphicsCamera::MouseMode _mouseMode;
    
    void _Initialize(int argc, char* argv[], std::string name, int init_width, int init_height);
    
    int _current_width;
    int _current_height;
    int _window_handle;
    std::string _window_name;

    static GraphicsWindow* _window;

private:

    GraphicsWindow(bool create);

    void makeInstance(int argc, char* argv[],
                      std::string name, int init_width, int init_height);
    
};

} // namespace akin

#endif // GRAPHICSWINDOW_H
