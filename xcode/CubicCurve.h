//
//  CubicCurve.h
//  DrawingToolkit
//
//  Created by Engage on 29/10/2014.
//
//

#ifndef __DrawingToolkit__CubicCurve__
#define __DrawingToolkit__CubicCurve__

#include <stdio.h>
#include <math.h>

#include "cinder/app/AppNative.h"

#define CAP_SEGMENT_WIDTH 1.0


#define MAX_LINE_WIDTH 5.0
#define MIN_LINE_WIDTH 0.25

using namespace ci;
using namespace std;
class CubicCurve
{
public:
    
    CubicCurve(Vec2f p0, Vec2f p1, Vec2f p2, Vec2f p3, float vStart, float vEnd);
    CubicCurve();
    
    void draw();
    void drawBeginningCap();
    void drawEndCap();
    void printInfo();
    
private:
    
    Vec2f   	*_vertices;
    Vec2f cubicCalc(Vec2f p0, Vec2f p1, Vec2f p2, Vec2f p3,float t);
    void drawCap(Vec2f a0, Vec2f a1, Vec2f a2, Vec2f a3, float angleStart);
    
    Vec2f	_anchor1;
    Vec2f	_anchor2;
    Vec2f	_control1;
    Vec2f	_control2;
    float   _angle;
    float   _lineWidthFront;
    float   _lineWidthEnd;
    
    vector <Vec2f> _drawPoints;
};

#endif 
