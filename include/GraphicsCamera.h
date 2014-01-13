
#ifndef GRAPHICSCAMERA_H
#define GRAPHICSCAMERA_H

#include <eigen3/Eigen/Core>
#include <eigen3/Eigen/Geometry>

#include <GL/glew.h>
#include <GL/glu.h>
#include <GL/glut.h>


class GraphicsCamera
{
public:

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


    /**
     * \brief Constructor for camera class
     */
    GraphicsCamera();

    Eigen::Isometry3f _modelview, _modelviewInv;
    Eigen::Matrix4f _projection, _projectionInv;
    Eigen::Vector3f _position, _target, _up;


    /**
     * Rotate scene on hemisphere. Project x,y coordinates
     * onto hemisphere and then compute angle between the
     * vectors and axis to rotate about and convert to
     * quaternion.
     */
    void rotateAboutWorld(const Eigen::Quaternionf& quat);
    void rotateAboutPoint(const Eigen::Vector3f& frame, const Eigen::Quaternionf& quat);
    void rotateAboutFrame(Eigen::Isometry3f& frame, const Eigen::Quaternionf& quat);
    void rotate(const Eigen::Quaternionf& quat);
    void rotate(int x, int y);
    void rotate(float x, float y, float z);
    void pan(float x, float y, float z);
    void pan(const Eigen::Vector3f& p);
    void zoom(float factor);
    void setCameraType(CameraType cameraType);
    void setViewport(int x, int y, int w, int h);
    void setPose(const Eigen::Isometry3f &pose);
    void setPose();
    void setPose(const Eigen::Vector3f& position, const Eigen::Vector3f& target, const Eigen::Vector3f& up);

    //==================================================
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    void setMousePosition(const Eigen::Vector2f &position);
    void updateView();
    Eigen::Vector3f hemisphereCoords(int x, int y) const;


private:

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
    Eigen::Vector2f _mouseXY;

    float _minSize, _maxSize;
    float _minXRot, _maxXRot;
    float _minYRot, _maxYRot;


    const Eigen::Isometry3f& getMatrix(MatrixType matrix_type);
    void loadMatrix(MatrixType matrix_type);
    const Eigen::Matrix4f& projection() const { return _projection; }
    const Eigen::Isometry3f& modelview() const { return _modelview; }

};

#endif // GRAPHICSCAMERA_H
