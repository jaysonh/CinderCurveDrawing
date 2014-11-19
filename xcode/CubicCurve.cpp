//
//  CubicCurve.cpp
//  DrawingToolkit
//
//  Created by Engage on 29/10/2014.
//
//

#include "CubicCurve.h"


CubicCurve::CubicCurve(Vec2f p0_, Vec2f p1_, Vec2f p2_, Vec2f p3_, float velocityFront_, float velocityEnd_)
{
    _anchor1  = p0_;
    _control1 = p1_;
    _control2 = p2_;
    _anchor2  = p3_;
    
    // Find the angle of curvature between the two control lines
    // this is used later to work out the density of segments when drawing the cubic shape
    // The tighter the curve the more segments we need.
    Vec2f d1 = _anchor1 - _control1;
    Vec2f d2 = _anchor2 - _control2;
    _angle = acosf( ((d1.x *d2.x) + (d1.y * d2.y)) / (d1.length()*d2.length()) );
    
    // Save the velocities for the end and beginning of the line (used for drawing the width)
    _lineWidthEnd   = velocityEnd_;
    _lineWidthFront = velocityFront_;
    
    
    _drawPoints.clear();
    // Work out how many lines are needed sections are 1 pixel long
    float sectionSize = _angle *0.5f;
    
    // Check if section size is NaN
    if(sectionSize != sectionSize)
        sectionSize = 1.0f;
    
    float dist = (_anchor2 - _anchor1).length();
    float inc = inc = sectionSize / dist;
    
    // Declared here so it can be used for final section of line
    Vec2f normal;
    for(float t = 0.0; t < 1.0f; t+= inc)
    {
        // Calc the width of the line at the given point t along 0 - 1.0
        float lineScale = t * (_lineWidthEnd- _lineWidthFront) + _lineWidthFront;
        Vec2f p1 = cubicCalc(_anchor1,_control1,_control2,_anchor2,t);
        Vec2f p2 = cubicCalc(_anchor1,_control1,_control2,_anchor2,t+inc);
        
        
        Vec2f diff = p2-p1;
        normal = Vec2f(-diff.y, diff.x).normalized();
        _drawPoints.push_back(p1 + normal * lineScale);
        _drawPoints.push_back(p1 - normal * lineScale);
    }
    
    // Draw final points of section
    Vec2f p1 = cubicCalc(_anchor1,_control1,_control2,_anchor2,1.0);
    _drawPoints.push_back(p1 + normal * _lineWidthEnd);
    _drawPoints.push_back(p1 - normal * _lineWidthEnd);
    
}

CubicCurve::CubicCurve()
{
    _anchor1 = Vec2f(0,0);
    _anchor2 = Vec2f(0,0);
    _control1 = Vec2f(0,0);
    _control2 = Vec2f(0,0);
}

void CubicCurve::printInfo()
{
    ci::app::console() << _anchor1 << " " << _control1 << " " << _control2 << " " << _anchor2 << std::endl;
}

void CubicCurve::draw()
{
    
    //gl::begin(GL_QUAD_STRIP);
    // Draw vertices
    for (std::vector<Vec2f>::iterator it = _drawPoints.begin(); it != _drawPoints.end() ; ++it)
    {
        gl::vertex((*it).x, (*it).y);
    }
    //gl::end();
    
    //ci::app::console() << _drawPoints[0] << " " << _drawPoints[_drawPoints.size()-1] << std::endl;
}

void CubicCurve::drawBeginningCap()
{
    // Calculate the dimenions of the caps
    Vec2f d1 = _control1 - _anchor1;
    Vec2f d2 = _control2 - _anchor2;
    
    Vec2f normalVec1 = Vec2f(-d1.y,d1.x).normalized();
    Vec2f normalVec2 = Vec2f(-d2.y,d2.x).normalized();
    
    // If either normals are Nan then recalculate using the anchor points
    if(normalVec2.x != normalVec2.x || normalVec1.x != normalVec1.x)
    {
        Vec2f d = _anchor1 - _anchor2;
        normalVec2 = Vec2f(-d.y,d.x).normalized();
        
        d = _anchor2 - _anchor1;
        normalVec1 = Vec2f(-d.y,d.x).normalized();
    }
    
    normalVec1 = normalVec1 * Vec2f(_lineWidthFront,_lineWidthFront);
    normalVec2 = normalVec2 * Vec2f(_lineWidthFront,_lineWidthFront);
    
        Vec2f a0 = _anchor1 + normalVec1;
        Vec2f a1 = _anchor1 - normalVec1;
        Vec2f a2 = (_anchor1 - normalVec1) - (_anchor2-_anchor1).normalized() * _lineWidthFront;
        Vec2f a3 = (_anchor1 + normalVec1) - (_anchor2-_anchor1).normalized() * _lineWidthFront;
        
        drawCap(a0,a1,a2,a3,0);
    
}

void CubicCurve::drawEndCap()
{
    // Calculate the dimenions of the caps
    Vec2f d1 = _control1 - _anchor1;
    Vec2f d2 = _control2 - _anchor2;
    
    Vec2f normalVec1 = Vec2f(-d1.y,d1.x).normalized();
    Vec2f normalVec2 = Vec2f(-d2.y,d2.x).normalized();
    
    // If either normals are Nan then recalculate using the anchor points
    if(normalVec2.x != normalVec2.x || normalVec1.x != normalVec1.x)
    {
        Vec2f d = _anchor1 - _anchor2;
        normalVec2 = Vec2f(-d.y,d.x).normalized();
        
        d = _anchor2 - _anchor1;
        normalVec1 = Vec2f(-d.y,d.x).normalized();
    }
    
    normalVec1 = normalVec1 * Vec2f(_lineWidthFront,_lineWidthFront);
    normalVec2 = normalVec2 * Vec2f(_lineWidthFront,_lineWidthFront);
    
        Vec2f a0 = _anchor2 + normalVec1;
        Vec2f a1 = _anchor2 - normalVec1;
        Vec2f a2 = (_anchor2 - normalVec1) - (_anchor1 - _anchor2).normalized() * _lineWidthEnd;
        Vec2f a3 = (_anchor2 + normalVec1) - (_anchor1 - _anchor2).normalized() * _lineWidthEnd;
        
        drawCap(a0,a1,a2, a3,M_PI);
    
}

void CubicCurve::drawCap(Vec2f a0, Vec2f a1, Vec2f a2, Vec2f a3, float angleStart)
{
    // Calculate the number of segments for the cap, the larger the cap then the more segments
    float capCircumference = (2.0 * M_PI * (a1 - a0).length()/2.0) / 2.0f;
    int   numCapSegments   = capCircumference / CAP_SEGMENT_WIDTH;
    
    float angleSepBetweenSegments = M_PI / (float)numCapSegments;
    
    Vec2f capCentre = (a1 - a0)/2.0f + a0;
    
    float capRadius = (a2 - a1).length();
    
    // Calculate the angle offset of the cap
    Vec2f diff = a1-a0;
    float angleOff = atan2f(diff.y, diff.x) + M_PI + angleStart;
    
    gl::begin(GL_POLYGON);
    for(float i = 0; i < numCapSegments+1; i+=1.0)
    {
        float angle = i * angleSepBetweenSegments + angleOff;
        gl::vertex(capCentre.x + capRadius * cos(angle), capCentre.y + capRadius * sin(angle));
    }
    gl::end();
}

// Calculate the position of a point for cubic p0, p1, p2, p3 for t (0.0-1.0) along the curve
Vec2f CubicCurve::cubicCalc(Vec2f p0, Vec2f p1, Vec2f p2, Vec2f p3,float t)
{
    float t1 = 1 - t;
    return p0*(t1*t1*t1) + p1*(3*t*t1*t1) + p2*(3*t*t*t1) + p3*(t*t*t);
   
}
