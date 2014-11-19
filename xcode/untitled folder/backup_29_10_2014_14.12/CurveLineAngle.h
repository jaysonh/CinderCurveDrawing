#ifndef CURVELINEANGLE_H
#define CURVELINEANGLE_H

#include "vector"
#include "deque"
#include "cinder/app/AppNative.h"
#include "cinder/Rand.h"
#include "cinder/Utilities.h"
#include "cinder/gl/Texture.h"
#include "cinder/Text.h"
#include "cinder/Vector.h"
#include "cinder/Path2d.h"
#include "cinder/gl/gl.h"
#include "CinderOpenCV.h"
#include "KalmanFilter.h"

#define SMOOTHNESS		0.4f
#define RAPIDNESS		0.4f

#define ANGLE_THRESH 0.2f

class CurveLineAngle
{
public:
    CurveLineAngle();

	
	void mouseDown(MouseEvent event,float kalmanSmoothness_,float kalmanRapidness_,float angleThresh_);
	void mouseUp(MouseEvent event);
	void mouseDrag(MouseEvent event, float minDistance);
	void update();

	void clear();
	void draw();
    
    void toggleKalmanFilter();
    void toggleDrawPointsPreKalman();
    void toggleDrawPointsPostKalman();
    void toggleDrawPointsAngleFilter();
    void toggleShowAnglePoints();
    
    bool isUsingKalmanFilter();
    bool isDrawPointsPreKalman();
    bool isDrawPointsPostKalman();
    bool isDrawPointsWithAngleFiltering();
    bool isShowAngleFilterPoints();
    
    int getNumPreKalmanPoints();
    int getNumPostKalmanPoints();
    int getNumAngleFilterPoints();
    
    void updateParams(float kalmanSmoothness_,float kalmanRapidness_,float angleThresh_);
private:
    void loadKalmanPoints();
    
    bool                            useKalmanFilter;
    bool                            drawPointsPreKalman;
    bool                            drawPointsPostKalman;
    bool                            drawPointsAngleFilterKalman;
    bool                            showAngleFilteredLinePoints;
	float							smoothness;
	float							rapidness;

    std::deque<Vec2f>               pointsPreKalman;
    std::deque<Vec2f>               pointsPostKalman;
    std::deque<Vec2f>               pointsAngleFilterKalman;
    

    float                           angleThresh;
	KalmanFilter					kalmanFilter;

};

#endif




