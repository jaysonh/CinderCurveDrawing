#include "CurveLineAngle.h"

CurveLineAngle::CurveLineAngle()
{
    
    useKalmanFilter             = true;
    drawPointsPreKalman         = true;
    drawPointsPostKalman        = true;
    drawPointsAngleFilterKalman = true;
    showAngleFilteredLinePoints = true;
}

void CurveLineAngle::updateParams(float kalmanSmoothness_,float kalmanRapidness_,float angleThresh_)
{
    angleThresh = angleThresh_;
    smoothness  = kalmanSmoothness_;
    rapidness   = kalmanRapidness_;
}

void CurveLineAngle::mouseDown(MouseEvent event,float kalmanSmoothness_,float kalmanRapidness_,float angleThresh_)
{
    ci::app::console() << "mousedown: " << kalmanSmoothness_ << " " << kalmanRapidness_ <<endl;
    angleThresh = angleThresh_;
	smoothness  = kalmanSmoothness_;
	rapidness   = kalmanRapidness_;

	kalmanFilter = KalmanFilter();
	kalmanFilter.init(event.getPos(), smoothness, rapidness);

    
    pointsPreKalman.clear();
    pointsPostKalman.clear();
    pointsAngleFilterKalman.clear();
    
    pointsPreKalman.push_back(event.getPos());
    pointsPostKalman.push_back(event.getPos());
    pointsAngleFilterKalman.push_back(event.getPos());
	
}
void CurveLineAngle::mouseUp(MouseEvent event)
{
	

}

void CurveLineAngle::toggleShowAnglePoints()
{
    showAngleFilteredLinePoints = !showAngleFilteredLinePoints;
}

void CurveLineAngle::toggleKalmanFilter()
{
    useKalmanFilter =  !useKalmanFilter;
}

void CurveLineAngle::toggleDrawPointsPreKalman()
{
    
    drawPointsPreKalman = !drawPointsPreKalman;
}
void CurveLineAngle::toggleDrawPointsPostKalman()
{
    drawPointsPostKalman =  !drawPointsPostKalman;
}
void CurveLineAngle::toggleDrawPointsAngleFilter()
{
    drawPointsAngleFilterKalman =  !drawPointsAngleFilterKalman;
}

void CurveLineAngle::loadKalmanPoints()
{
    if(pointsPreKalman.size() > 0)
    {
        pointsAngleFilterKalman.clear();
        pointsPostKalman.clear();
        kalmanFilter.init(pointsPreKalman[0],smoothness,rapidness);
        for (std::deque<Vec2f>::const_iterator it = pointsPreKalman.begin(); it != pointsPreKalman.end(); ++it)
        {
            //kalmanFilter.update(event.getPos());
            kalmanFilter.update(*it);
            Vec2f kalmanEstimatedPos = kalmanFilter.getEstimation();
            pointsPostKalman.push_back(kalmanEstimatedPos);
            
            if (pointsAngleFilterKalman.size() >= 2)
            {
                Vec2f lastPoint1 = pointsAngleFilterKalman[pointsAngleFilterKalman.size() - 1];
                Vec2f lastPoint2 = pointsAngleFilterKalman[pointsAngleFilterKalman.size() - 2];
                Vec2f newPoint ;
                
                newPoint = kalmanFilter.getEstimation();
                
                float angle1 = atan2f(newPoint.y - lastPoint1.y, newPoint.x - lastPoint1.x);
                float angle2 = atan2f(lastPoint1.y - lastPoint2.y, lastPoint1.x - lastPoint2.x);
                
                float angleDiff =abs(angle1 - angle2);
                
                if ( angleDiff<  angleThresh)
                    pointsAngleFilterKalman[pointsAngleFilterKalman.size() - 1] = newPoint;
                else
                    pointsAngleFilterKalman.push_back(newPoint);
                
            }
            else
            {
                
                pointsAngleFilterKalman.push_back(*it);
            }
        }
    }
}

void CurveLineAngle::mouseDrag(MouseEvent event, float minDistance)
{   
    pointsPreKalman.push_back(event.getPos());
    
    
	
	
}
void CurveLineAngle::update()
{
	

}

void CurveLineAngle::clear()
{
    pointsPreKalman.clear();
    pointsPostKalman.clear();
    pointsAngleFilterKalman.clear();
}



void CurveLineAngle::draw()
{

    loadKalmanPoints();
    gl::lineWidth(1.0f);

    if(drawPointsPreKalman)
    {
        gl::color(Color(1, 0, 1));
        gl::begin(GL_LINE_STRIP);
        for (std::deque<Vec2f>::const_iterator it = pointsPreKalman.begin(); it != pointsPreKalman.end(); ++it)
        {
            gl::vertex(it->x, it->y);
        }
        gl::end();
    }
    if(drawPointsPostKalman)
    {
        gl::color(Color(0, 1, 1));
        gl::begin(GL_LINE_STRIP);
        for (std::deque<Vec2f>::const_iterator it = pointsPostKalman.begin(); it != pointsPostKalman.end(); ++it)
        {
            gl::vertex(it->x, it->y);
        }
        gl::end();
    }
    if(drawPointsAngleFilterKalman)
    {
        gl::color(Color(1, 1, 1));
        gl::begin(GL_LINE_STRIP);
        for (std::deque<Vec2f>::const_iterator it = pointsAngleFilterKalman.begin(); it != pointsAngleFilterKalman.end(); ++it)
        {
            gl::vertex(it->x, it->y);
        }
        gl::end();
        
        if(showAngleFilteredLinePoints)
        {
            gl::color(Color(0, 1, 0));
            for (std::deque<Vec2f>::const_iterator it = pointsAngleFilterKalman.begin(); it != pointsAngleFilterKalman.end(); ++it)
            {
                gl::drawSolidCircle(*it,2);
            }
        }
    }
    
}

bool CurveLineAngle::isUsingKalmanFilter()
{
    return useKalmanFilter;
}

bool CurveLineAngle::isDrawPointsPreKalman()
{
    return drawPointsPreKalman;
}

bool CurveLineAngle::isDrawPointsPostKalman()
{
    return drawPointsPostKalman;
}

bool CurveLineAngle::isDrawPointsWithAngleFiltering()
{
    return drawPointsAngleFilterKalman;
}

bool CurveLineAngle::isShowAngleFilterPoints()
{
    return showAngleFilteredLinePoints;
}


int CurveLineAngle::getNumPreKalmanPoints()
{
    return pointsPreKalman.size();
}

int CurveLineAngle::getNumPostKalmanPoints()
{
    return pointsPostKalman.size();
    
}

int CurveLineAngle::getNumAngleFilterPoints()
{
    
    return pointsAngleFilterKalman.size();
}