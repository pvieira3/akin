#include "Picker.h"
#include "LineCallback.h"

Picker::Picker() : _mX(0.), _mY(0.)
{

}

bool Picker::handle(const osgGA::GUIEventAdapter &ea, osgGA::GUIActionAdapter &aa)
{
    osgViewer::Viewer* viewer = dynamic_cast<osgViewer::Viewer*>(&aa);
    if(!viewer) {
        return false;
    }

    switch(ea.getEventType()) {
        case osgGA::GUIEventAdapter::PUSH:
        case osgGA::GUIEventAdapter::MOVE: {
            std::cerr << "Got move" << std::endl;
            _mX = ea.getX();
            _mY = ea.getY();
            return false;
        }
        case osgGA::GUIEventAdapter::RELEASE: {
            std::cerr << "Got release" << std::endl;
            if(_mX == ea.getX() && _mY == ea.getY()) {
                if(Picker::pick(ea.getXnormalized(), ea.getYnormalized(), viewer)) {
                    std::cerr << "returning true" << std::endl;
                    return true;
                }
            }
            std::cerr << "returning false" << std::endl;
            return false;
        }
        default: {
            return false;
        }
    }
}


bool Picker::pick( const double x, const double y, osgViewer::Viewer* viewer )
{
     if (!viewer->getSceneData()) {
        // Nothing to pick.
        return false;
     }

     double w( .005 ), h( .005 );
     osgUtil::PolytopeIntersector* picker =
     new osgUtil::PolytopeIntersector(osgUtil::Intersector::PROJECTION,
                                        x-w, y-h, x+w, y+h );

     osgUtil::IntersectionVisitor iv( picker );
     viewer->getCamera()->accept( iv );

     if (picker->containsIntersections()) {
         std::cerr << "Picker contains intersections" << std::endl;
         const osg::NodePath& nodePath = picker->getFirstIntersection().nodePath;
         unsigned int idx = nodePath.size();
         while (idx--) {
             // Find the LAST MatrixTransform in the node
             // path; this will be the MatrixTransform
             // to attach our callback to.
             osg::MatrixTransform* mt =
             dynamic_cast<osg::MatrixTransform*>(nodePath[ idx ]);
             if (mt == NULL) {
                continue;
             }

             // If we get here, we just found a
             // MatrixTransform in the nodePath.
             if (_selectedNode.valid())
             // Clear the previous selected node's
             // callback to make it stop spinning.
             _selectedNode->setUpdateCallback( NULL );

             _selectedNode = mt;

             _selectedNode->setUpdateCallback( new LineCallback );
             break;
        }
         if (!_selectedNode.valid()) {
            osg::notify() << "Pick failed." << std::endl;
         }
     }
     else if (_selectedNode.valid()) {
         _selectedNode->setUpdateCallback( NULL );
         _selectedNode = NULL;
     }
     return _selectedNode.valid();
}
