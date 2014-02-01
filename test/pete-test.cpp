#include <osg/Node>
#include <osg/Group>
#include <osg/Geode>
#include <osg/Geometry>
#include <osg/Texture2D>
#include <osgDB/ReadFile> 
#include <osgViewer/Viewer>
#include <osg/PositionAttitudeTransform>
#include <osgGA/TrackballManipulator>
#include <osg/MatrixTransform>
#include <osg/ShapeDrawable>
#include <osgViewer/CompositeViewer>
#include <LineCallback.h>
#include <AkinCallback.h>
#include "Picker.h"

// readInStl
#include <osgDB/ReadFile>


osgViewer::Viewer* createViewer(int x, int y, int w, int h, osg::Node* scene)
{
    osg::ref_ptr<osgViewer::Viewer> viewer = new osgViewer::Viewer;
    viewer->setSceneData(scene);
    viewer->setUpViewInWindow(x, y, w, h);

    // Create camera with no inertia and add it to the viewer.
    osgGA::OrbitManipulator* camera = new osgGA::TrackballManipulator();
    camera->setAllowThrow(false);
    viewer->setCameraManipulator(camera);
    viewer->getCamera()->setClearColor(osg::Vec4(0.5, 0.5, 0.5, .5));
    viewer->addEventHandler(new Picker);
    viewer->realize();

    while( !viewer->done() )
    {
        viewer->frame();
    }
}

osgViewer::View* createView(int x, int y, int w, int h, osg::Node* scene)
{
    osgViewer::View* view = new osgViewer::View;
    osgGA::OrbitManipulator* cameraManipulator = new osgGA::OrbitManipulator();
    cameraManipulator->setAllowThrow(false);
    view->setCameraManipulator(cameraManipulator);
    view->getCamera()->setClearColor(osg::Vec4(0.5, 0.5, 0.5, 0.5));
    view->setUpViewInWindow(x, y, w, h);
    view->setSceneData(scene);
    return view;
}

int callbackTest()
{
    osg::Group* root = new osg::Group();

    // Create cylinder
    osg::Geode* geode = new osg::Geode;
    osg::TessellationHints* hints = new osg::TessellationHints;
    osg::ShapeDrawable* cylinder = new osg::ShapeDrawable(new osg::Cylinder(osg::Vec3(0.0f, 0.0f, 0), .01, 1), hints);
    cylinder->setColor(osg::Vec4(1.0f, 0.0f, 0.0f, 1.0f));
    geode->addDrawable(cylinder);
    osg::Geode* geode2 = new osg::Geode;
    osg::ShapeDrawable* cylinder2 = clone(cylinder);
    cylinder2->setColor(osg::Vec4(0.0f, 0.0f, 1.0f, 1.0f));
    geode2->addDrawable(cylinder2);

    // Create dynamics TF
    osg::MatrixTransform* rootTF = new osg::MatrixTransform;
    osg::MatrixTransform* tf1 = new osg::MatrixTransform;
    osg::MatrixTransform* tf2 = new osg::MatrixTransform;
    tf1->setDataVariance(osg::Object::DYNAMIC);
//    tf1->setUpdateCallback(new LineCallback);
    tf2->setDataVariance(osg::Object::DYNAMIC);
//    tf2->setUpdateCallback(new LineCallback);
    osg::Matrix m;
    m.makeTranslate(0.6, 0, 0);
    tf1->setMatrix(m);
    tf1->addChild(geode);
    tf2->addChild(geode2);

    // Add TF to root
    rootTF->addChild(tf1);
    rootTF->addChild(tf2);
    root->addChild(rootTF);

    // switch off lighting as we haven't assigned any normals.
    root->getOrCreateStateSet()->setMode(GL_LIGHTING, osg::StateAttribute::OFF);

    createView(0, 0, 640, 480, root);

    return 0;
}

int readInStl()
{
    osg::Group* root = new osg::Group();
    osg::Group* root2 = new osg::Group();
    osg::MatrixTransform* rootTF = new osg::MatrixTransform;
    osg::MatrixTransform* rootTF2 = new osg::MatrixTransform;
    osg::MatrixTransform* tf1 = new osg::MatrixTransform;
    osg::MatrixTransform* tf2 = new osg::MatrixTransform;
    tf1->setDataVariance(osg::Object::DYNAMIC);
    tf2->setDataVariance(osg::Object::DYNAMIC);

    rootTF->addChild(tf1);
    rootTF2->addChild(tf2);
    osg::Matrix m;
    m.makeTranslate(0.6, 0, 0);
    tf2->setMatrix(m);
    root->addChild(rootTF);
    root2->addChild(rootTF2);


    osg::Node* stl1 = osgDB::readNodeFile("/home/pete/catkin_ws/src/hubo_gt/urdf/drchubo_v2/meshes/convhull_LAP.stl");
    if(!stl1) {
        std::cerr << "Failed to load file stl1" << std::endl;
        exit(-1);
    }

    osg::Node* stl2 = osgDB::readNodeFile("/home/pete/catkin_ws/src/hubo_gt/urdf/drchubo_v2/meshes/convhull_LKP.stl");
    if(!stl2) {
        std::cerr << "Failed to load file stl2" << std::endl;
        exit(-1);
    }

    tf1->addChild(stl1);
    tf2->addChild(stl2);



    osgViewer::CompositeViewer* mainViewer = new osgViewer::CompositeViewer();
    mainViewer->addView(createView(100, 0, 640, 480, root));
    mainViewer->addView(createView(0, 0, 50, 50, root2));
    mainViewer->run();
    return 0;
}


int main(int argc, char** argv)
{
    //callbackTest();
    readInStl();
    return 0;
}
