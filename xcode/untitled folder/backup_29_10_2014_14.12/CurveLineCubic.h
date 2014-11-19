#ifndef CURVELINECUBIC_H
#define CURVELINECUBIC_H

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
#define CUBIC_FILTER	0.5f

typedef struct
{
	Vec2f	anchor1;
	Vec2f	anchor2;

	Vec2f	control1;
	Vec2f	control2;

} CubicCurve;



class CurveLineCubic
{
public:

	
	void mouseDown(MouseEvent event);
	void mouseUp(MouseEvent event);
	void mouseDrag(MouseEvent event, float minDistance);
	void update();

	void clear();
	void draw();
private:
	void drawCurve(CubicCurve *c);
	Vec2f calcQuadratic(Vec2f p0, Vec2f p1, Vec2f p2, float t);

	float							smoothness;
	float							rapidness;

	float							minDistance;
	std::deque<Vec2f>				pointsDeque;
	std::deque<QuadraticCurve>		quadraticCurvesDeque;
	std::vector<CubicCurve>			cubicCurves;


	std::deque<Vec2f>				frontPointsDeque;

	KalmanFilter					kalmanFilter;

};

#endif




