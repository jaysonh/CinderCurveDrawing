#ifndef CURVE_H
#define CURVE_H

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
#include "CubicCurve.h"
#include "TouchVec2f.h"

enum DrawToolType { PEN, ERASER };

#define SMOOTHNESS		0.4f
#define RAPIDNESS		0.4f

#define ANGLE_THRESH 0.2f

#define MAX_LINE_WIDTH 4.0f
#define MIN_LINE_WIDTH 2.0f

#define LINE_SIZE_MULTIPLIER 25.0f

#define KALMAN_SMOOTHNESS 0.0001f
#define KALMAN_RAPIDNESS  0.001f
#define CUBIC_NORMAL_MULT 0.67f

class Curve
{
public:
    Curve();
    Curve(float r, float g, float b, DrawToolType tool);

    void mouseDown(Vec2f pos);
	void mouseUp(Vec2f pos);
	void mouseDrag(Vec2f pos, float minDistance);
	void update();

	void clear();
	void draw();
    
    int getNumCubics();
    int getNumQuadratics();
    void updateParams(float kalmanSmoothness_,float kalmanRapidness_,float angleThresh_,float cubicNormalMultiplier_);
    void setColour(float r, float g, float b);
    
    int getNumSections();
private:
    
    Vec2f getMidPoint(Vec2f p0, Vec2f p1);
    
    bool                            _finishedDrawing;
    bool                            _recalculateCurve;
    DrawToolType                    _tool;
    float                           _r, _g, _b;
    
    float							_smoothness;
	float							_rapidness;
    float                           _cubicNormalMultiplier;
    
    std::vector<TouchVec2f>         _pointsPreKalman;
    std::vector<TouchVec2f>         _pointsPostKalman;
    std::vector<CubicCurve>         _cubicCurveLines;

    float                           _lineWidth;
	KalmanFilter					_kalmanFilter;

};

#endif




