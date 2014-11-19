#include "vector"
#include "deque"
#include "cinder/ip/Resize.h"
#include "cinder/app/AppNative.h"
#include "cinder/Rand.h"
#include "cinder/Utilities.h"
#include "cinder/gl/Texture.h"
#include "cinder/Text.h"
#include "cinder/Utilities.h"
#include "cinder/ImageIo.h"
#include "cinder/Font.h"
#include "cinder/Vector.h"
#include "cinder/Path2d.h"
#include "cinder/gl/gl.h"
#include "CinderOpenCV.h"
#include "cinder/gl/Fbo.h"
#include "cinder/Font.h"
#include "Curve.h"
#include "KalmanFilter.h"
#include "cinder/params/Params.h"
#include "boost/date_time/posix_time/posix_time.hpp"
#include "DrawingToolKit.h"




using namespace ci;
using namespace ci::app;
using namespace std;


class DrawingToolkitApp : public AppNative
{

public:
	DrawingToolkitApp();
	void setup();
	void mouseDown(MouseEvent event);
	void mouseUp(MouseEvent event);
	void mouseDrag(MouseEvent event);
	void keyDown(KeyEvent event);

	void update();
	void draw();
    
    void resize(Event e);
private:
    
    Vec2i lastScreenDimenions;
    DrawingToolKit drawingToolKit;
    
};

DrawingToolkitApp::DrawingToolkitApp()
{

}

void DrawingToolkitApp::resize(Event e)
{
}

void DrawingToolkitApp::setup()
{

    lastScreenDimenions = getWindowSize();
    drawingToolKit.init(getWindowSize().x,getWindowSize().y);
}

void DrawingToolkitApp::mouseDown(MouseEvent event)
{
    drawingToolKit.beginTouch(event.getPos(),0);
}

void DrawingToolkitApp::mouseUp(MouseEvent event)
{
    drawingToolKit.endTouch(event.getPos(),0);

}

void DrawingToolkitApp::mouseDrag(MouseEvent event)
{
    drawingToolKit.moveTouch(event.getPos(),0);
}

void DrawingToolkitApp::keyDown(KeyEvent event)
{
	if (event.getChar() == ' ')
		drawingToolKit.clear();
    if(event.getChar() == 'c')
        drawingToolKit.setColour(Rand::randFloat(0.0,1.0f),Rand::randFloat(0.0,1.0f),Rand::randFloat(0.0,1.0f));

}

void DrawingToolkitApp::update()
{
    if(lastScreenDimenions.x != getWindowSize().x || lastScreenDimenions.y != getWindowSize().y)
        drawingToolKit.setSize(getWindowSize().x,getWindowSize().y);
    drawingToolKit.update();
	

    
}

void DrawingToolkitApp::draw()
{

	gl::clear(Color(0.5, 0.5, 0.5));
    
    drawingToolKit.draw();
    
    ci::app::console() << "Frame rate: " << ci::app::getFrameRate() << std::endl;
}



CINDER_APP_NATIVE(DrawingToolkitApp, RendererGl)
