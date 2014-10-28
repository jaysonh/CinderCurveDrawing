#include "cinder/app/AppNative.h"
#include "cinder/gl/gl.h"

using namespace ci;
using namespace ci::app;
using namespace std;

class DrawingToolkitApp : public AppNative {
  public:
	void setup();
	void mouseDown( MouseEvent event );	
	void update();
	void draw();
};

void DrawingToolkitApp::setup()
{
}

void DrawingToolkitApp::mouseDown( MouseEvent event )
{
}

void DrawingToolkitApp::update()
{
}

void DrawingToolkitApp::draw()
{
	// clear out the window with black
	gl::clear( Color( 0, 0, 0 ) ); 
}

CINDER_APP_NATIVE( DrawingToolkitApp, RendererGl )
