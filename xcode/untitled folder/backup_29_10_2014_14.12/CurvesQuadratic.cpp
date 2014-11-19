#include "CurvesQuadratic.h"



CurvesQuadratic::CurvesQuadratic()
{
   
}
void CurvesQuadratic::mouseDown(MouseEvent event)
{
    isMouseDown=true;
	smoothness = SMOOTHNESS;
	rapidness = RAPIDNESS;

	kalmanFilter = KalmanFilter();
	kalmanFilter.init(event.getPos(), smoothness, rapidness);

	pointsDeque.clear();
	pointsDeque.push_back(event.getPos());

	pointsRaw.clear();
	pointsRaw.push_back(event.getPos());

}
void CurvesQuadratic::mouseUp(MouseEvent event)
{
    isMouseDown=false;
    /*Quadratic saveQuad1 = Quadratic(endQuadratic);
    
    if(pointsDeque.size()>= 2)
    {
        Quadratic tipQuad = Quadratic(pointsDeque[pointsDeque.size()-2],pointsDeque[pointsDeque.size()-2],pointsDeque[pointsDeque.size()-1],false);
       
        quadraticList.push_back(tipQuad);
    }
    quadraticList.push_back(saveQuad1);
   
    
    quadraticList.push_back(Quadratic(endQuadratic.p1,endQuadratic.p2,endQuadratic.p3,false));*/

}
void CurvesQuadratic::mouseDrag(MouseEvent event, float minDistance)
{

	kalmanFilter.update(event.getPos());
	Vec2f p = kalmanFilter.getEstimation();
	pointsRaw.push_back(p);

	if (pointsDeque.size() > 1)
	{
		Vec2f lastPoint1 = pointsDeque[pointsDeque.size() - 1];
		Vec2f lastPoint2 = pointsDeque[pointsDeque.size() - 2];
		Vec2f newPoint  = event.getPos();
		float angle1 = atan2f(newPoint.y - lastPoint1.y, newPoint.x - lastPoint1.x);
		float angle2 = atan2f(lastPoint1.y - lastPoint2.y, lastPoint1.x - lastPoint2.x);


		if (abs(angle1 - angle2) < ANGLE_THRESH )
		{
			pointsDeque[pointsDeque.size() - 1] = newPoint;
		}
		else
			pointsDeque.push_back(newPoint);

	}
	else
	{

		pointsDeque.push_back(event.getPos());
	}
	
}
void CurvesQuadratic::update()
{
	/*while(pointsDeque.size() >= 3)
	{
        
		Quadratic q = Quadratic(pointsDeque[0], pointsDeque[1], pointsDeque[2]);
		quadraticList.push_back(q);

		pointsDeque.pop_front();
	}*/
    
    
    
    
}

void CurvesQuadratic::clear()
{
	pointsDeque.clear();
	pointsRaw.clear();
	quadraticList.clear();
    
    endQuadratic = Quadratic(Vec2f(0,0),Vec2f(0,0),Vec2f(0,0));
}



void CurvesQuadratic::draw()
{

	gl::lineWidth(5.0f);

	
	/*gl::color(Color(1, 1, 1));
	gl::lineWidth(1.0f);
	gl::begin(GL_LINE_STRIP);
	for (std::deque<Vec2f>::const_iterator it = pointsDeque.begin(); it != pointsDeque.end(); ++it)
	{
		gl::vertex(it->x, it->y);
	}
	gl::end();

	gl::color(Color(0, 1, 0));
	for (std::deque<Vec2f>::const_iterator it = pointsDeque.begin(); it != pointsDeque.end(); ++it)
	{
		gl::drawSolidCircle(*it,5);
	}
    
    gl::lineWidth(5.0f);
	gl::color(Color(1.0, 1.0, 0.0));
	for (std::vector<Quadratic>::const_iterator it = quadraticList.begin(); it != quadraticList.end(); ++it)
	{
		Quadratic q = (*it);
		q.draw();
	}*/
    if(isMouseDown)
    {
        /*gl::begin(GL_LINE_STRIP);
        for (std::deque<Vec2f>::const_iterator it = pointsRaw.begin(); it != pointsRaw.end(); ++it)
        {
            gl::vertex(it->x, it->y);
        }
        gl::end();*/
        gl::begin(GL_LINE_STRIP);
        for (std::deque<Vec2f>::const_iterator it = pointsDeque.begin(); it != pointsDeque.end(); ++it)
        {
            gl::vertex(it->x, it->y);
        }
        gl::end();
    }else
    {
    ci::app::console() << pointsDeque.size() << endl;
    if(pointsDeque.size() >= 2)
    {
        Vec2f p1 = pointsDeque[0];
        Vec2f p2 = pointsDeque[1];
        Vec2f p3 = pointsDeque[1];
        Quadratic q = Quadratic(p1,p2,p3,true,false);
        
        q.draw();
    }
    if(pointsDeque.size() >= 3)
    {
        for (int i = 0; i < pointsDeque.size()-3; i++)
        {
            Vec2f p1 = pointsDeque[i+0];
            Vec2f p2 = pointsDeque[i+1];
            Vec2f p3 = pointsDeque[i+2];
            Quadratic q = Quadratic(p1,p2,p3);
        
            q.draw();
        }
        
        // Draw end
        Vec2f p1 = pointsDeque[pointsDeque.size()-3];
        Vec2f p2 = pointsDeque[pointsDeque.size()-2];
        Vec2f p3 = pointsDeque[pointsDeque.size()-1];
        Quadratic q = Quadratic(p1,p2,p3,false,true);
        
        q.draw();
    }
    }
    
    /*gl::color(Color(1.0, 0.0, 0.0));
	endQuadratic.draw();
    
    gl::color(Color(0.0, 1.0, 0.0));
    if(pointsDeque.size()>= 2)
    {
        Quadratic tipQuad = Quadratic(pointsDeque[pointsDeque.size()-2],pointsDeque[pointsDeque.size()-2],pointsDeque[pointsDeque.size()-1],false);
        tipQuad.draw();
        
    }*/
    
	//ci::app::console() << "Num Raw Points: " << pointsRaw.size() << " num points with angle thresh: " << pointsDeque.size() << std::endl;
}