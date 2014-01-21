#ifndef LINECALLBACK_H
#define LINECALLBACK_H

#include <osg/Node>
#include <osg/NodeCallback>
#include "ObjectManipDataType.h"
#include <osg/MatrixTransform>
#include <iostream>
#include <osg/io_utils>

class LineCallback : public osg::NodeCallback
{
public:
    LineCallback()
    {

    }

    virtual void operator()(osg::Node* node, osg::NodeVisitor* nv)
    {
        osg::ref_ptr<osg::MatrixTransform> tf =
                dynamic_cast<osg::MatrixTransform*> (node);
        if(tf) {
            osg::Matrix m;
            m.makeRotate(5*M_PI/180, 0, 1, 0);
            tf->preMult(m);
        }

        traverse(node, nv);
    }
};

#endif // LINECALLBACK_H
