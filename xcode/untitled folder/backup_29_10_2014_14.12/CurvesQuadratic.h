#ifndef CURVESQUADRATIC_H
#define CURVESQUADRATIC_H

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
#include "Quadratic.h"

#define SMOOTHNESS		0.4f
#define RAPIDNESS		0.4f

#define ANGLE_THRESH 0.2

class CurvesQuadratic
{
public:
	CurvesQuadratic();
	
	void mouseDown(MouseEvent event);
	void mouseUp(MouseEvent event);
	void mouseDrag(MouseEvent event, float minDistance);
	void update();

	void clear();
	void draw();
private:
    bool                            isMouseDown;
	Quadratic						endQuadratic;
    
	float							smoothness;
	float							rapidness;

	std::deque<Vec2f>				pointsDeque;
	std::deque<Vec2f>				pointsRaw;

	std::vector<Quadratic>			quadraticList;

	KalmanFilter					kalmanFilter;

};

#endif




