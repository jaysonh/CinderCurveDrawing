#include "Curve.h"

Curve::Curve()
{
    Curve(0.0, 1.0, 0.0,PEN);
}
Curve::Curve(float r, float g, float b, DrawToolType tool)
{
    _recalculateCurve = true;
    _tool = tool;
    _r = r;
    _g = g;
    _b = b;
    
    // This is the maximum line width
    
    _smoothness              = KALMAN_SMOOTHNESS;
    _rapidness               = KALMAN_RAPIDNESS;
    _cubicNormalMultiplier   = CUBIC_NORMAL_MULT;
    
   
}

void Curve::updateParams(float kalmanSmoothness_,float kalmanRapidness_,float angleThresh_, float cubicNormalMultiplier_)
{
    _smoothness              = kalmanSmoothness_;
    _rapidness               = kalmanRapidness_;
    _cubicNormalMultiplier   = cubicNormalMultiplier_;
}

void Curve::mouseDown(Vec2f pos)
{
    _finishedDrawing = false;
	_kalmanFilter = KalmanFilter();
	_kalmanFilter.init(pos, _smoothness, _rapidness);

    _cubicCurveLines.clear();
    _pointsPreKalman.clear();
    _pointsPostKalman.clear();
    
    Vec2f adjustedPos = pos;// - Vec2f(ci::app::getWindowWidth()/2.0, ci::app::getWindowHeight()/2.0f);
   
    
    // Need to makes sure first and second points are atleast lineWidth distance away from each other
    
    _pointsPreKalman.push_back(TouchVec2f(adjustedPos));
    _pointsPostKalman.push_back(TouchVec2f(adjustedPos));
    _recalculateCurve=true;
}

Vec2f Curve::getMidPoint(Vec2f p0, Vec2f p1)
{
    return ((p1-p0)/2.0) + p0;
}
void Curve::mouseUp(Vec2f pos)
{
    _finishedDrawing = true;
}

void Curve::mouseDrag(Vec2f pos, float minDistance)
{
    TouchVec2f lastPos = _pointsPreKalman.back().p;
    float distToLastPoint = (pos - lastPos.p).length();
    if(distToLastPoint > 1.0f)
    {
        float vel = (1.0f / distToLastPoint) * LINE_SIZE_MULTIPLIER;
        
        if(vel > MAX_LINE_WIDTH) vel = MAX_LINE_WIDTH;
        if(vel < MIN_LINE_WIDTH) vel = MIN_LINE_WIDTH;
        
        if(_pointsPreKalman.size() >= 2)
        {
            // scale compared to previous velocity
            float velDiff = (_pointsPreKalman.back().v * 0.75 + vel * 0.25) ;
            vel = velDiff;
        }
        _pointsPreKalman.push_back(TouchVec2f(pos,vel));
    }
    _recalculateCurve=true;
}

void Curve::update()
{
    if(_recalculateCurve)
    {
        // Run raw points list through kalman filter
        if(_pointsPreKalman.size() > 0)
        {
            _pointsPostKalman.clear();
            _pointsPostKalman.reserve(_pointsPreKalman.size());
            _kalmanFilter.init(_pointsPreKalman.front().p,_smoothness,_rapidness);
            
            for (std::vector<TouchVec2f>::const_iterator itPos = _pointsPreKalman.begin(); itPos != _pointsPreKalman.end() ; ++itPos)
            {
                _kalmanFilter.update((*itPos).p);
                
                
                    Vec2f kalmanEstimatedPos = _kalmanFilter.getEstimation();
                    _pointsPostKalman.push_back(TouchVec2f(kalmanEstimatedPos, (*itPos).v));
                
            }
        }
        
        
        if(_pointsPostKalman.size() >= 2)
        {
            _cubicCurveLines.clear();
            _cubicCurveLines.reserve(_pointsPostKalman.size());
            
            // Add beginning section
            _cubicCurveLines.push_back(CubicCurve(_pointsPostKalman[0].p,
                                                 getMidPoint(_pointsPostKalman[0].p,_pointsPostKalman[1].p),
                                                 getMidPoint(_pointsPostKalman[0].p,_pointsPostKalman[1].p),
                                                 getMidPoint(_pointsPostKalman[0].p,_pointsPostKalman[1].p),
                                                 _pointsPostKalman[2].v,
                                                 _pointsPostKalman[2].v));
            
            for (std::vector<TouchVec2f>::const_iterator itPos = _pointsPostKalman.begin()+2; itPos != _pointsPostKalman.end() ; ++itPos)
            {
                Vec2f p0 = (itPos-2)->p;
                Vec2f p1 = (itPos-1)->p;
                Vec2f p2 = (itPos)->p;
                
                float velocityFront = (itPos-1)->v;
                float velocityEnd   = (itPos)->v;
                
                if(itPos == _pointsPostKalman.begin()+2)
                    velocityFront = velocityEnd;
                if(itPos == _pointsPostKalman.end()-1)
                    velocityEnd = velocityFront;
                
                
                Vec2f q0 = getMidPoint(p0,p1);
                Vec2f q1 = p1;
                Vec2f q2 = p1;
                Vec2f q3 = getMidPoint(p1,p2);
                
                q1 = q0 + (q1-q0)*_cubicNormalMultiplier;
                q2 = q2 + (q3-q2)*(1-_cubicNormalMultiplier);
                
                CubicCurve newCubic(q0,
                                    q1,
                                    q2,
                                    q3,
                                    velocityFront,
                                    velocityEnd);
                
                
                _cubicCurveLines.push_back(newCubic);
                
                // If at the end then add the end section
                if(itPos == _pointsPostKalman.end()-1)
                {
                    _cubicCurveLines.push_back(CubicCurve(getMidPoint(p1,p2),
                                                         getMidPoint(p1,p2),
                                                         getMidPoint(p1,p2),
                                                         p2,
                                                         velocityFront,
                                                         velocityEnd));
                }
            }
            
        }else if(_pointsPostKalman.size() ==1)
        {
            
            // draw a single point
            Vec2f off = Vec2f(Rand::randFloat(-1,1),Rand::randFloat(-1,1));
            _cubicCurveLines.push_back(CubicCurve(_pointsPostKalman[0].p- off,
                                                  _pointsPostKalman[0].p,
                                                  _pointsPostKalman[0].p,
                                                  _pointsPostKalman[0].p+off,
                                                  Rand::randFloat(MIN_LINE_WIDTH,MAX_LINE_WIDTH),
                                                  Rand::randFloat(MIN_LINE_WIDTH,MAX_LINE_WIDTH)));
        }
        _recalculateCurve=false;
    }
}

void Curve::clear()
{
    _recalculateCurve=true;
    _cubicCurveLines.clear();
    _pointsPreKalman.clear();
    _pointsPostKalman.clear();
}

void Curve::setColour(float r, float g, float b)
{
    _r = r;
    _g = g;
    _b = b;
}

void Curve::draw()
{    
        gl::color(ColorA(_r,_g,_b, 1.0f));
    
        gl::begin(GL_QUAD_STRIP);
        for (std::vector<CubicCurve>::iterator it = _cubicCurveLines.begin(); it != _cubicCurveLines.end(); ++it)
        {            
                (*it).draw();
        }
        gl::end();
    
        // Draw the beginning and end sections
        if(_cubicCurveLines.size()>0)
        {
            _cubicCurveLines[0].drawBeginningCap();
            _cubicCurveLines.back().drawEndCap();
        }
    
}

int Curve::getNumSections()
{
    return _cubicCurveLines.size();
}
