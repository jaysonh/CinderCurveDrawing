#include "CurveLineQuadMid.h"



void CurveLineQuadMid::mouseDown(MouseEvent event)
{
	firstSection = true;
	smoothness = SMOOTHNESS;
	rapidness = RAPIDNESS;

	kalmanFilter = KalmanFilter();
	kalmanFilter.init(event.getPos(), smoothness, rapidness);

	pointsDeque.clear();
	pointsDeque.push_back(event.getPos());

	frontPointsDeque.clear();
	frontPointsDeque.push_back(event.getPos());

}
void CurveLineQuadMid::mouseUp(MouseEvent event)
{
	// Create  section for last bit
	QuadraticCurve	curve;

	if (frontPointsDeque.size() > 0)
	{
		int midPointIndx = 0;
		if (frontPointsDeque.size() > 1)
			midPointIndx = frontPointsDeque.size() / 2;

		curve.p1 = frontPointsDeque[0];
		curve.p2 = frontPointsDeque[midPointIndx];
		curve.p3 = frontPointsDeque[frontPointsDeque.size() - 1];
		quadraticCurvesDeque.push_back(curve);
	}
	frontPointsDeque.clear();
	//ci::app::console() << " num points left: " <<  pointsDeque.size() << std::endl;

}
void CurveLineQuadMid::mouseDrag(MouseEvent event, float minDistance)
{

	kalmanFilter.update(event.getPos());

	if (pointsDeque.back().distance(event.getPos()) > minDistance)
	{
		pointsDeque.push_back(kalmanFilter.getEstimation());
		frontPointsDeque.push_back(kalmanFilter.getEstimation());

	}
	else
	{
		frontPointsDeque.push_back(kalmanFilter.getEstimation());
	}
	
}
void CurveLineQuadMid::update()
{
	while (pointsDeque.size() > 2)
	{
		QuadraticCurve	curve;

		curve.p1 = getMidPoint(pointsDeque[0], pointsDeque[1]);
		curve.p2 = pointsDeque[1];
		curve.p3 = getMidPoint(pointsDeque[1], pointsDeque[2]);

		// If there are no quadratics, create one for first section
		if (firstSection)
		{
			firstSection = false;
			QuadraticCurve	firstCurve;

			firstCurve.p1 = pointsDeque[0];
			firstCurve.p2 = getMidPoint(pointsDeque[0], pointsDeque[1]);
			firstCurve.p3 = getMidPoint(pointsDeque[0], pointsDeque[1]);

			quadraticCurvesDeque.push_front (firstCurve);

		}
		//ci::app::console() << curve.p1 << " " << curve.p2 << " " << curve.p3 <<  std::endl;

		quadraticCurvesDeque.push_back(curve);

		pointsDeque.pop_front();
		frontPointsDeque.clear();
		frontPointsDeque.push_front(curve.p3);
			
	}


}

Vec2f CurveLineQuadMid::getMidPoint(Vec2f p1, Vec2f p2)
{
	return Vec2f((p2.x - p1.x) / 2.0 + p1.x, (p2.y - p1.y) / 2.0 + p1.y);
}
void CurveLineQuadMid::clear()
{
	pointsDeque.clear();
	quadraticCurvesDeque.clear();
	

}


Vec2f CurveLineQuadMid::calcQuadratic(Vec2f p0, Vec2f p1, Vec2f p2, float t)
{
	return Vec2f((1 - t) * (1 - t) * p0.x + 2 * (1 - t) * t * p1.x + t * t * p2.x,
		(1 - t) * (1 - t) * p0.y + 2 * (1 - t) * t * p1.y + t * t * p2.y);

}


void CurveLineQuadMid::draw()
{

	ci::app::console() << "Quad Curve List size: " << quadraticCurvesDeque.size() << std::endl;
	gl::lineWidth(5.0f);

	//gl::color( ColorA( 0, 1, 0, 0.5f ) );
	gl::color(Color(0, 1, 0));

	for (std::deque<QuadraticCurve>::const_iterator it = quadraticCurvesDeque.begin(); it != quadraticCurvesDeque.end(); ++it)

	{
		
		Path2d	path;

		path.moveTo(it->p1);
		
		path.quadTo(it->p2, it->p3);

		gl::draw(path);
	}


	gl::lineWidth(5.0f);
	gl::color(Color(0, 1, 0));
	gl::begin(GL_LINE_STRIP);
	for (std::deque<Vec2f>::const_iterator it = frontPointsDeque.begin(); it != frontPointsDeque.end(); ++it)
	{
		gl::vertex(it->x, it->y);
	}
	gl::end();

}