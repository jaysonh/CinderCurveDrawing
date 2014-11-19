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
#include "CurveLineCubic.h"
#include "CurveLineQuadMid.h"
#include "CurveLineAngle.h"
#include "CurvesQuadratic.h"
#include "KalmanFilter.h"
#include "CurveDrawer.h"
#include "cinder/params/Params.h"
#include "boost/date_time/posix_time/posix_time.hpp"

#define MIN_DISTANCE    20.0f

using namespace ci;
using namespace ci::app;
using namespace std;

enum CURVE_MODE { QUADRATIC, CUBIC, ANGLE, QUADANGLE };

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


private:
    params::InterfaceGlRef          mParams;
    float                           kalmanSmoothness;
    float                           kalmanRapidness;
    float                           angleThresh;
	Font							font;
    
	CURVE_MODE curveMode;
	
	CurveLineQuadMid				curveLinesQuad;
	CurveLineCubic					curveLinesCubic;
	CurveLineAngle					curveLinesAngle;
	CurveDrawer                     curvesQuadAngle;
bool							saveFBO;
	gl::Fbo							fbo;
    
    void drawInfoPanel();
    void toggleKalmanFilter();
    void toggleDrawPointsPreKalman();
    void toggleDrawPointsPostKalman();
    void toggleDrawPointsAngleFilter();
    void toggleShowAngleFilterPoints();
    void saveScreenshot();
};

DrawingToolkitApp::DrawingToolkitApp()
{

}

void DrawingToolkitApp::saveScreenshot()
{
    saveFBO = true;
}

void DrawingToolkitApp::setup()
{
	gl::enableAlphaBlending(); 

	saveFBO = false;
	curveMode = ANGLE;

    // Default values
    kalmanSmoothness = 0.0001f;
    kalmanRapidness  = 0.1f;
    angleThresh      = 0.12f;
    font = Font("Arial", 12.0f);
    mParams = params::InterfaceGl::create( getWindow(), "parameters", toPixels( Vec2i( 340, 200 ) ) );
    mParams->addParam( "Kalman Smoothness", &kalmanSmoothness ).min( 0.00000f ).max( 2.5f ).precision( 5 ).step( 0.0001f );
    mParams->addParam( "Kalman Rapidness", &kalmanRapidness ).min( 0.00000f ).max( 2.5f ).precision( 4 ).step( 0.01f );
    mParams->addParam( "Angle Thresh", &angleThresh ).min( 0.00000f ).max( 1.0 ).precision( 2 ).step( 0.01f );
    mParams->addButton("Toggle KalmanFilter", std::bind( &DrawingToolkitApp::toggleKalmanFilter, this ) );
    mParams->addButton("Draw Points Pre Kalman", std::bind( &DrawingToolkitApp::toggleDrawPointsPreKalman, this ) );
    mParams->addButton("Draw Points Post Kalman", std::bind( &DrawingToolkitApp::toggleDrawPointsPostKalman, this ) );
    mParams->addButton("Draw Points With Angle Filtering", std::bind( &DrawingToolkitApp::toggleDrawPointsAngleFilter, this ) );
    mParams->addButton("Show Angle Filter Points", std::bind( &DrawingToolkitApp::toggleShowAngleFilterPoints, this ) );
    mParams->addButton("Save Screenshot", std::bind( &DrawingToolkitApp::saveScreenshot, this ) );
    
}

void DrawingToolkitApp::toggleShowAngleFilterPoints()
{
    curveLinesAngle.toggleShowAnglePoints();
}

void DrawingToolkitApp::toggleDrawPointsPreKalman()
{
    curveLinesAngle.toggleDrawPointsPreKalman();
}

void DrawingToolkitApp::toggleDrawPointsPostKalman()
{
    
    curveLinesAngle.toggleDrawPointsPostKalman();
}
void DrawingToolkitApp::toggleDrawPointsAngleFilter()
{
    curveLinesAngle.toggleDrawPointsAngleFilter();
    
}
void DrawingToolkitApp::toggleKalmanFilter()
{
    curveLinesAngle.toggleKalmanFilter();
}
void DrawingToolkitApp::mouseDown(MouseEvent event)
{
    ci::app::console() << event.getPos() << endl;
	switch (curveMode)
	{
	case QUADRATIC:
		curveLinesQuad.mouseDown(event);
		break;

	case CUBIC:
		curveLinesCubic.mouseDown(event);
		break;

	case ANGLE:
		curveLinesAngle.mouseDown(event,kalmanSmoothness,kalmanRapidness,angleThresh);
		break;

	case QUADANGLE:
		curvesQuadAngle.mouseDown(event);
		break;
	}

}

void DrawingToolkitApp::mouseUp(MouseEvent event)
{
	
		switch (curveMode)
		{
		case QUADRATIC:
			curveLinesQuad.mouseUp(event);
			break;

		case CUBIC:
			curveLinesCubic.mouseUp(event);
			break;

		case ANGLE:
			curveLinesAngle.mouseUp(event);
			break;

		case QUADANGLE:
			curvesQuadAngle.mouseUp(event);
			break;
		}

}

void DrawingToolkitApp::mouseDrag(MouseEvent event)
{
	switch (curveMode)
	{
	case QUADRATIC:
		curveLinesQuad.mouseDrag(event, MIN_DISTANCE);
		break;

	case CUBIC:
		curveLinesCubic.mouseDrag(event, MIN_DISTANCE);
		break;

	case ANGLE:
		curveLinesAngle.mouseDrag(event, MIN_DISTANCE);
		break;

	case QUADANGLE:
		curvesQuadAngle.mouseDrag(event, MIN_DISTANCE);
		break;
	}

}

void DrawingToolkitApp::keyDown(KeyEvent event)
{
	if (event.getChar() == ' ')
	{ 
		curvesQuadAngle.clear();
		curveLinesAngle.clear();
		curveLinesCubic.clear();
		curveLinesQuad.clear();
	}
	 if (event.getChar() == 's' || event.getChar() == 'S')
	{
		saveFBO = true;
	}

	 if (event.getChar() == '1')
		 curveMode = QUADRATIC;
	 if (event.getChar() == '2')
		 curveMode = CUBIC;
	 if (event.getChar() == '3')
		 curveMode = ANGLE;
	 if (event.getChar() == '4')
		 curveMode = QUADANGLE;
}

void DrawingToolkitApp::update()
{
	
	switch (curveMode)
	{
	case QUADRATIC:
		curveLinesQuad.update();
		break;

	case CUBIC:
		curveLinesCubic.update();
		break;

	case ANGLE:
        curveLinesAngle.updateParams(kalmanSmoothness,kalmanRapidness,angleThresh);
		curveLinesAngle.update();
		break;

	case QUADANGLE:
		curvesQuadAngle.update();
		break;
	}

}

void DrawingToolkitApp::draw()
{

	gl::clear(Color(0, 0, 0));

	switch (curveMode)
	{
		case QUADRATIC:
		curveLinesQuad.draw();
		break;

		case CUBIC:
			curveLinesCubic.draw();
			break;

		case ANGLE:
			curveLinesAngle.draw();
			break;

		case QUADANGLE:
			curvesQuadAngle.draw();
			break;
	}


    drawInfoPanel();
    mParams->draw();
	if (saveFBO)
	{
		saveFBO = false;
		writeImage("screnshot" + toString(boost::posix_time::second_clock::universal_time()) + ".png", copyWindowSurface());
	}
    
}

void DrawingToolkitApp::drawInfoPanel()
{
    gl::pushMatrices();
    
    gl::translate(Vec2f(350,00));
    gl::color(Color(1, 1, 1));
    gl::drawString("Framerate: " + toString(getAverageFps()), Vec2f(10, 20.0f), Color::white(), font);
    gl::drawString("Num Pre Kalman Points:       " + toString(curveLinesAngle.getNumPreKalmanPoints()), Vec2f(10, 35.5f), Color::white(), font);
    gl::drawString("Num Post Kalman Points:       " + toString(curveLinesAngle.getNumPostKalmanPoints()), Vec2f(10, 50.5f), Color::white(), font);
    gl::drawString("Num Angle Filter Points:       " + toString(curveLinesAngle.getNumAngleFilterPoints()), Vec2f(10, 65.5f), Color::white(), font);
    
    gl::drawString("Using Kalman Filter:       " + toString(curveLinesAngle.isUsingKalmanFilter()), Vec2f(10, 87.5f), Color::white(), font);
    gl::drawString("Draw Points Pre Kalman:    " + toString(curveLinesAngle.isDrawPointsPreKalman()), Vec2f(10, 102.5f), Color(1, 0, 1), font);
    
    gl::drawString("Draw Points Post Kalman:   " + toString(curveLinesAngle.isDrawPointsPostKalman()), Vec2f(10, 117.5f), Color(0, 1, 1), font);
    gl::drawString("Draw Points With Ang Filt: " + toString(curveLinesAngle.isDrawPointsWithAngleFiltering()), Vec2f(10, 132.5f), Color(1, 1, 1), font);
    gl::drawString("Show Angle Filter Points:  " + toString(curveLinesAngle.isShowAngleFilterPoints()), Vec2f(10, 147.5f), Color::white(), font);
    
    
    gl::popMatrices();
}


CINDER_APP_NATIVE(DrawingToolkitApp, RendererGl)
