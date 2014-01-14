#include "GraphicsCamera.h"
#include "glutils.h"
#include <iostream>
#include <math.h>

using namespace akin;

GraphicsCamera* GraphicsCamera::_camera = 0;

GraphicsCamera::GraphicsCamera(verbosity::verbosity_level_t report_level) :
    frame(Frame::World(), "camera", verbosity::INHERIT)
{
    verb.level = report_level;
    if(verb.level == verbosity::INHERIT) {
        verb.level = verbosity::LOG;
    }

    _makeCamera(verb.level);
}

GraphicsCamera::GraphicsCamera(bool create, verbosity::verbosity_level_t report_level) :
    frame(Frame::World(), "camera", verbosity::INHERIT)
{
    //setViewport(0, 0, 100, 100);

    verb.level = report_level;
}

void GraphicsCamera::_makeCamera(verbosity::verbosity_level_t report_level)
{
    if(_camera == 0) {
        verb.debug() << "Camera does not exist yet. Creating one now!"; verb.end();
        _camera = new GraphicsCamera(true, report_level);
        verb.debug() << "Static camera created."; verb.end();
    }
}

void GraphicsCamera::rotate(int x, int y)
{
    Eigen::Vector3f cur, origin;

//    std::cerr << "\n\n"
//              << "xy: " << x << ", " << y
//              << "mouse: " << _mouseXY.x() << ", " << _mouseXY.y() << "\n\n";

    cur = hemisphereCoords(x,y);
    origin = hemisphereCoords(_camera->_mouseXY.x(), _camera->_mouseXY.y());

//    std::cerr << "x, y = " << x << ", " << y << "\n"
//              << "cur = " << cur.transpose() << "\n"
//              << "ori = " << origin.transpose() << "\n";

    Eigen::Quaternionf quat;
//    Eigen::Vector3f axis;
//    float angle;
//    axis = (origin.cross(cur)).normalized();

//    angle = axis.norm() / (origin.norm() * cur.norm());
    quat.setFromTwoVectors(origin, cur);
//    std::cerr << "angle-axis: " << angle << ", " << axis.transpose() << std::endl;
//    std::cerr << "quat      : " << quat.vec().transpose() << std::endl;
//    Eigen::Matrix3f mat = quat.matrix();

    glMatrixMode(GL_MODELVIEW);
    rotateAboutWorld(quat);
   // updateView();
}

void GraphicsCamera::rotate(const Eigen::Quaternionf& quat)
{
    _camera->_modelview.rotate(quat);
    loadMatrix(MATRIX_MODELVIEW);
    glutPostRedisplay();
}

void GraphicsCamera::rotateAboutWorld(const Eigen::Quaternionf &quat)
{
//    Eigen::Isometry3f world = Eigen::Isometry3f::Identity();
//    rotateAboutFrame(world, quat);
    _camera->_modelview.rotate(quat);
    loadMatrix(MATRIX_MODELVIEW);
    glutPostRedisplay();
}

void GraphicsCamera::rotateAboutFrame(Eigen::Isometry3f& frame, const Eigen::Quaternionf& quat)
{
    Eigen::Isometry3f newPose = frame * quat * _camera->_modelview;
    std::cerr << " quat = " << quat.vec().transpose() << "\n"
              << "_pose = \n" << _camera->_modelview.matrix() << "\n"
              << "newPz = \n" << newPose.matrix() << std::endl;
    setPose(newPose);

}

void GraphicsCamera::rotateAboutPoint(const Eigen::Vector3f& frame, const Eigen::Quaternionf& quat)
{
//    Eigen::Vector3f newCameraPos = _position - frame;
    //_position = frame + newCameraPos*(quat);
}

void GraphicsCamera::rotate(float x, float y, float z)
{
    glRotatef(x, 1, 0, 0);
    glRotatef(y, 0, 1, 0);
    glRotatef(z, 0, 0, 1);
    updateView();
}

void GraphicsCamera::pan(float x, float y, float scale)
{
    pan(Eigen::Vector3f(scale*x,scale*y,0));
}

void GraphicsCamera::pan(const Eigen::Vector3f& p)
{
    _camera->_modelview.pretranslate(p);
    loadMatrix(MATRIX_MODELVIEW);
    glutPostRedisplay();
}

void GraphicsCamera::zoom(float factor)
{
    _camera->_modelview.pretranslate(Eigen::Vector3f(0, 0, factor));
    loadMatrix(MATRIX_MODELVIEW);
    glutPostRedisplay();

}

void GraphicsCamera::setCameraType(CameraType cameraType)
{
    _camera->_cameraType = cameraType;
}

void GraphicsCamera::setPose(const Eigen::Isometry3f& pose)
{
    _camera->_position = pose.translation();
    Eigen::Matrix3f rot = pose.rotation();
    _camera->_target << rot(2,0), rot(2,1), rot(2,2);
    _camera->_up << rot(1,0), rot(1,1), rot(1,2);
    _camera->_modelview = pose;
}

void GraphicsCamera::setPose(const Eigen::Vector3f& position, const Eigen::Vector3f& target, const Eigen::Vector3f& up)
{
    // Set camera position, target and up vectors
    _camera->_position = position;
    _camera->_target = target;
    _camera->_up = up;

    // Set camera pose
        // Set translate part to position of camera in world
    _camera->_modelview.setIdentity();
    _camera->_modelview.translate(Eigen::Vector3f(position.x(), position.y(), -position.z()));
        // Create rotation part from position, target and up vectors
    Eigen::Vector3f xVec, yVec, zVec;
    zVec = position - target;
    xVec = up.cross(zVec);
    yVec = zVec.cross(xVec);
        // Create rotation matrix and rotate camera pose
    Eigen::Matrix3f rot;
    rot.col(0) = xVec.normalized();
    rot.col(1) = yVec.normalized();
    rot.col(2) = zVec.normalized();
    _camera->_modelview.rotate(rot);
//    std::cerr << "_modelview:\n" << _modelview.matrix() << std::endl;
}

void GraphicsCamera::setPose()
{
    GLfloat m[16];
    glGetFloatv(GL_MODELVIEW_MATRIX, m);
    _camera->_modelview = glutils::glToMat4(m);
//    std::cerr << "_modelview:\n" << _modelview.matrix() << std::endl;
//    std::cerr << "_modelviewInv:\n" << _modelview.inverse().matrix() << std::endl;
}

//--------------------------
//    MOUSE FUNCTIONS
//--------------------------

void GraphicsCamera::mousePressed(int x, int y, GraphicsCamera::MouseMode mouseMode)
{
    std::cerr << "Mouse: " << mouseModeStr(mouseMode) << "\n";

    _camera->_mouseMode = mouseMode;
    _camera->_mouseXY << x, y;

    switch(mouseMode) {
        case MOUSE_SCROLL_UP: {
            zoom(1);
            break;
        }
        case MOUSE_SCROLL_DOWN: {
            zoom(-1);
            break;
        }
        default: break;
    }
}


void GraphicsCamera::mouseReleased(int x, int y, GraphicsCamera::MouseMode mouseMode)
{
    _camera->_mouseMode = MOUSE_NONE;
}

void GraphicsCamera::mouseMoved(int x, int y, GraphicsCamera::MouseMode mouseMode)
{
    // How did the mouse move?
    int dx = x - _camera->_mouseXY.x();
    int dy = y - _camera->_mouseXY.y();

    // Move camera based on mouse mode and mouse motion
    switch(mouseMode) {
        case MOUSE_PAN: {
            pan(dx, -dy);
            break;
        }
        case MOUSE_ZOOM: {
            zoom(dy);
            break;
        }
        case MOUSE_ROTATE: {
            rotate(x, y); // Virtual Trackball
            break;
        }
        case MOUSE_SCROLL_UP: {
            zoom(1);
            break;
        }
        case MOUSE_SCROLL_DOWN: {
            zoom(-1);
            break;
        }
    }

    _camera->_mouseXY << x, y;
}

void GraphicsCamera::mouseReset()
{
    _camera->_mouseMode = MOUSE_NONE;
}



//--------------------------
// Private Member Functions
//--------------------------

void GraphicsCamera::setViewport(int x, int y, int w, int h)
{
    _camera->_viewport.x = x;
    _camera->_viewport.y = y;
    _camera->_viewport.w = w;
    _camera->_viewport.h = h;
    _camera->_viewport.center.x() = (w - x)/2;
    _camera->_viewport.center.y() = (h - y)/2;
    _camera->_viewport.diagonal = sqrt((w-x)*(w-x) + (h-y)*(h-y));
}

void GraphicsCamera::setMousePosition(const Eigen::Vector2f& position)
{
    _camera->_mouseXY = position;
}

Eigen::Vector3f GraphicsCamera::hemisphereCoords(int x, int y)
{
    Eigen::Vector3f ray; // Ray in hemisphere going from world origin

    // Compute ray components by first getting radius of hemishpere which
    // is half the diagonal of the viewport
    float r = _camera->_viewport.diagonal/2;
    ray.x() = x - _camera->_viewport.center.x();
    ray.y() = _camera->_viewport.center.y() - y;
    ray.z() = std::max(0.0, sqrt(r*r - ray.x()*ray.x() - ray.y()*ray.y()));

    return ray;
}

void GraphicsCamera::loadMatrix(MatrixType matrix_type)
{
    GLfloat glMat[16];
    glutils::iso3ToGl(getMatrix(matrix_type), glMat);
    glutils::loadMatrix(glMat);
//    std::cerr << "modelview\n" << _modelview.matrix() << std::endl;
//    std::cerr << "modelviewInv\n" << _modelview.inverse().matrix() << "\n\n";
}

const Eigen::Isometry3f& GraphicsCamera::getMatrix(MatrixType matrix_type)
{
    switch(matrix_type) {
    case MATRIX_PROJECTION:
        //return _projection;
    case MATRIX_MODELVIEW:
    default:
        return _camera->_modelview;
    }
}

void GraphicsCamera::updateView()
{
    std::cout << "_position: " << _camera->_position.transpose() << "\n"
              <<  " _target: " << _camera->_target.transpose() << "\n"
              << "      _up: " << _camera->_up.transpose()
              << std::endl;
    gluLookAt(_camera->_position.x(), _camera->_position.y(), _camera->_position.z(),
              _camera->_target.x(), _camera->_target.y(), _camera->_target.z(),
              _camera->_up.x(), _camera->_up.y(), _camera->_up.z());
    glutPostRedisplay();
}
