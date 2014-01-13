#include "glutils.h"
#include <iostream>

namespace glutils
{

void iso3ToGl(const Eigen::Isometry3f& m, GLfloat* glMat)
{
    glutils::mat4ToGl(m.matrix(), glMat);
}

void mat4ToGl(const Eigen::Matrix4f& m, GLfloat* glMat)
{
    glMat[0] = m(0,0); glMat[4] = m(0,1); glMat[8] = m(0,2);  glMat[12] = m(0,3);
    glMat[1] = m(1,0); glMat[5] = m(1,1); glMat[9] = m(1,2);  glMat[13] = m(1,3);
    glMat[2] = m(2,0); glMat[6] = m(2,1); glMat[10] = m(2,2); glMat[14] = m(2,3);
    glMat[3] = m(3,0); glMat[7] = m(3,1); glMat[11] = m(3,2); glMat[15] = m(3,3);
}

Eigen::Matrix4f glToMat4(GLfloat* glMat)
{
    Eigen::Matrix4f m;
    m(0,0) = glMat[0]; m(0,1) = glMat[4]; m(0,2) = glMat[8];  m(0,3) = glMat[12];
    m(1,0) = glMat[1]; m(1,1) = glMat[5]; m(1,2) = glMat[9];  m(1,3) = glMat[13];
    m(2,0) = glMat[2]; m(2,1) = glMat[6]; m(2,2) = glMat[10]; m(2,3) = glMat[14];
    m(3,0) = glMat[3]; m(3,1) = glMat[7]; m(3,2) = glMat[11]; m(3,3) = glMat[15];
    return m;
}

} // end glutils namespace
