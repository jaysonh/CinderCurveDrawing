#ifndef CURVELINEQUADMID_H
#define CURVELINEQUADMID_H

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

#define SMOOTHNESS		0.1f
#define RAPIDNESS		0.1f

#define ANGLE_THRESH 

#define CUBIC_FILTER	0.5f




class CurveLineQuadMid
{
public:
	void mouseDown(MouseEvent event);
	void mouseUp(MouseEvent event);
	void mouseDrag(MouseEvent event, float minDistance);
	void update();

	void clear();
	void draw();
private:

	Vec2f calcQuadratic(Vec2f p0, Vec2f p1, Vec2f p2, float t);
	Vec2f getMidPoint(Vec2f p1, Vec2f p2);

	float							smoothness;
	float							rapidness;

	float							minDistance;
	std::deque<Vec2f>				pointsDeque;
	std::deque<QuadraticCurve>		quadraticCurvesDeque;


	std::deque<Vec2f>				frontPointsDeque;

	KalmanFilter					kalmanFilter;
	bool firstSection;
};

#endif




