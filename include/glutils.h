#ifndef GLUTILS_H
#define GLUTILS_H

#include <GL/gl.h>
#include <GL/freeglut.h>
#include <eigen3/Eigen/Core>
#include <eigen3/Eigen/Geometry>

namespace glutils
{

inline void loadMatrix(GLfloat* data) { /*glLoadMatrixf(data);*/ }
inline void multMatrix(GLfloat* matrix) { /*glMultMatrixf(matrix);*/ }


void mat4ToGl(const Eigen::Matrix4f& m, GLfloat* glMat);
void iso3ToGl(const Eigen::Isometry3f& m, GLfloat* glMat);
Eigen::Matrix4f glToMat4(GLfloat* glMat);

} // end glutils namespace

#endif // GLUTILS_H
