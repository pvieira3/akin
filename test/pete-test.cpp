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
#include <LineCallback.h>
#include <AkinCallback.h>

osgViewer::Viewer* createView(int x, int y, int w, int h, osg::Node* scene)
{
    osg::ref_ptr<osgViewer::Viewer> view = new osgViewer::Viewer;
    view->setSceneData(scene);
    view->setUpViewInWindow(x, y, w, h);
    return view.release();
}

int main()
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
    tf1->setUpdateCallback(new LineCallback);
    tf2->setDataVariance(osg::Object::DYNAMIC);
    tf2->setUpdateCallback(new LineCallback);
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

    // Set up viewer in window
    osgViewer::Viewer viewer;
    viewer.setSceneData(root);
    viewer.setUpViewInWindow(0, 0, 640, 480);

    // Create camera with no inertia and add it to the viewer.
    osgGA::OrbitManipulator* camera = new osgGA::TrackballManipulator();
    camera->setAllowThrow(false);
    viewer.setCameraManipulator(camera);
    viewer.getCamera()->setClearColor(osg::Vec4(0.5, 0.5, 0.5, .5));
    viewer.realize();

    while( !viewer.done() )
    {
        viewer.frame();
    }

    return 0;
}
