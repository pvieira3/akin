#include <osgGA/GUIEventHandler>
#include <osgGA/GUIEventAdapter>
#include <osgViewer/Viewer>

class Picker : public osgGA::GUIEventHandler
{
public:
    Picker();
    osg::ref_ptr<osg::Node> _selectedNode;

    bool handle(const osgGA::GUIEventAdapter &ea, osgGA::GUIActionAdapter &aa);

protected:
    float _mX, _mY;
    bool pick(const double x, const double y, osgViewer::Viewer* viewer);
};
