
#ifndef GRAPHICSCAMERA_H
#define GRAPHICSCAMERA_H

#include <eigen3/Eigen/Core>
#include <eigen3/Eigen/Geometry>

#include <GL/glew.h>
#include <GL/glu.h>
#include <GL/glut.h>

#include "IncludeGL.h"
#include "AkinIncludes.h"

namespace akin {

class GraphicsCamera
{
public:


    /**
     * \brief Constructor for camera class
     */
    GraphicsCamera(verbosity::verbosity_level_t report_level = verbosity::LOG);

    enum CameraType {
        CAMERA_ORTHONORMAL,
        CAMERA_PERSPECTIVE
    };

    enum MatrixType {
        MATRIX_PROJECTION,
        MATRIX_MODELVIEW
    };

    enum MouseMode {
        MOUSE_NONE=0,
        MOUSE_PAN,
        MOUSE_ZOOM,
        MOUSE_ROTATE,
        MOUSE_SCROLL_UP,
        MOUSE_SCROLL_DOWN
    };

    static inline std::string mouseModeStr(GraphicsCamera::MouseMode mouseMode)
    {
        switch(mouseMode) {
            case MOUSE_NONE:        return "MOUSE_NONE";
            case MOUSE_PAN:         return "MOUSE_PAN";
            case MOUSE_ZOOM:        return "MOUSE_ZOOM";
            case MOUSE_ROTATE:      return "MOUSE_ROTATE";
            case MOUSE_SCROLL_UP:   return "MOUSE_SCROLL_UP";
            case MOUSE_SCROLL_DOWN: return "MOUSE_SCROLL_DOWN";
            default:
                return "Error: Invalid mouse mode!";
        }
    }

    Frame frame;
    Eigen::Isometry3f _modelview, _modelviewInv;
    Eigen::Matrix4f _projection, _projectionInv;
    Eigen::Vector3f _position, _target, _up;


    /**
     * Rotate scene on hemisphere. Project x,y coordinates
     * onto hemisphere and then compute angle between the
     * vectors and axis to rotate about and convert to
     * quaternion.
     */
    static void rotateAboutWorld(const Eigen::Quaternionf& quat);
    static void rotateAboutPoint(const Eigen::Vector3f& frame, const Eigen::Quaternionf& quat);
    static void rotateAboutFrame(Eigen::Isometry3f& frame, const Eigen::Quaternionf& quat);
    static void rotate(const Eigen::Quaternionf& quat);
    static void rotate(int x, int y);
    static void rotate(float x, float y, float z);
    static void pan(float x, float y, float scale=0.01);
    static void pan(const Eigen::Vector3f& p);
    static void zoom(float factor);
    static void setCameraType(CameraType cameraType);
    static void setViewport(int x, int y, int w, int h);
    static void setPose(const Eigen::Isometry3f &pose);
    static void setPose();
    static void setPose(const Eigen::Vector3f& position, const Eigen::Vector3f& target, const Eigen::Vector3f& up);

    //========== Mouse Functions ==================
    static void mousePressed(int x, int y, GraphicsCamera::MouseMode mouseMode);
    static void mouseReleased(int x, int y, GraphicsCamera::MouseMode mouseMode);
    static void mouseMoved(int x, int y, GraphicsCamera::MouseMode mouseMode);
    static void mouseReset();
    static Eigen::Vector3f hemisphereCoords(int x, int y);
    static void setMousePosition(const Eigen::Vector2f &position);


    static void updateView();

    verbosity verb;


protected:

    class Viewport
    {
    public:
        int x;
        int y;
        int w;
        int h;
        float diagonal;
        Eigen::Vector2f center;
    };

    Viewport _viewport;
    CameraType _cameraType;
    MouseMode _mouseMode;
    Eigen::Vector2f _mouseXY;

    float _minSize, _maxSize;
    float _minXRot, _maxXRot;
    float _minYRot, _maxYRot;


    static const Eigen::Isometry3f& getMatrix(MatrixType matrix_type);
    static void loadMatrix(MatrixType matrix_type);
    static const Eigen::Matrix4f& projection() { return _camera->_projection; }
    static const Eigen::Isometry3f& modelview() { return _camera->_modelview; }

    static GraphicsCamera* _camera;

    void _makeCamera(verbosity::verbosity_level_t report_level);

private:

    GraphicsCamera(bool create, verbosity::verbosity_level_t report_level);

}; // end GraphicsCamera class

} // end akin namespace

#endif // GRAPHICSCAMERA_H
