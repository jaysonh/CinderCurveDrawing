#include "CurveLineCubic.h"



void CurveLineCubic::mouseDown(MouseEvent event)
{
	smoothness = SMOOTHNESS;
	rapidness = RAPIDNESS;

	kalmanFilter = KalmanFilter();
	kalmanFilter.init(event.getPos(), smoothness, rapidness);

	pointsDeque.clear();
	pointsDeque.push_back(event.getPos());


	frontPointsDeque.clear();
	frontPointsDeque.push_back(event.getPos());

}
void CurveLineCubic::mouseUp(MouseEvent event)
{
	if (frontPointsDeque.size() > 0)
	{
		QuadraticCurve	curveQuad;

		int midPointIndx = 0;
		if (frontPointsDeque.size() > 1)
			midPointIndx = frontPointsDeque.size() / 2;

		curveQuad.p1 = frontPointsDeque[0];
		curveQuad.p2 = frontPointsDeque[midPointIndx];
		curveQuad.p3 = frontPointsDeque[frontPointsDeque.size() - 1];
		//quadraticCurvesDeque.push_back(curve);
		CubicCurve	curve;
		curve.anchor1 = curveQuad.p1;
		curve.anchor2 = curveQuad.p3;

		Vec2f normal1 = (curveQuad.p2 - curveQuad.p1).normalized();
		float distance1 = curveQuad.p2.distance(curveQuad.p1);

		Vec2f normal2 = (curveQuad.p2 - curveQuad.p3).normalized();
		float distance2 = curveQuad.p2.distance(curveQuad.p3);

		curve.control1 = curveQuad.p1 + normal1 * distance1 * CUBIC_FILTER;
		curve.control2 = curveQuad.p3 + normal2 * distance2 * CUBIC_FILTER;

		if (cubicCurves.size() > 0)
		{
			Vec2f normal = (cubicCurves.back().control2 - cubicCurves.back().anchor2).normalized().inverse();

			float distance = curve.control1.distance(curve.anchor1);

			curve.control1 = curve.anchor1 + normal * distance;
		}

		cubicCurves.push_back(curve);
	}

}
void CurveLineCubic::mouseDrag(MouseEvent event, float minDistance)
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
void CurveLineCubic::update()
{
	while (pointsDeque.size() > 2)
	{
		QuadraticCurve	curve;

		curve.p1 = pointsDeque[0];
		curve.p2 = pointsDeque[1];
		curve.p3 = pointsDeque[2];

		quadraticCurvesDeque.push_back(curve);

		pointsDeque.pop_front();
		pointsDeque.pop_front();

		
	}

	// Quadratic to Cubic calculation
	while (quadraticCurvesDeque.size())
	{

		CubicCurve	curve;

		curve.anchor1 = quadraticCurvesDeque.front().p1;
		curve.anchor2 = quadraticCurvesDeque.front().p3;

		Vec2f normal1 = (quadraticCurvesDeque.front().p2 - quadraticCurvesDeque.front().p1).normalized();
		float distance1 = quadraticCurvesDeque.front().p2.distance(quadraticCurvesDeque.front().p1);

		Vec2f normal2 = (quadraticCurvesDeque.front().p2 - quadraticCurvesDeque.front().p3).normalized();
		float distance2 = quadraticCurvesDeque.front().p2.distance(quadraticCurvesDeque.front().p3);

		curve.control1 = quadraticCurvesDeque.front().p1 + normal1 * distance1 * CUBIC_FILTER;
		curve.control2 = quadraticCurvesDeque.front().p3 + normal2 * distance2 * CUBIC_FILTER;

		if (cubicCurves.size() > 0)
		{
			Vec2f normal = (cubicCurves.back().control2 - cubicCurves.back().anchor2).normalized().inverse();

			float distance = curve.control1.distance(curve.anchor1);

			curve.control1 = curve.anchor1 + normal * distance;
		}

		cubicCurves.push_back(curve);

		quadraticCurvesDeque.pop_front();


		frontPointsDeque.clear();
		
		// Need to put last point from cubic curve into front points

		frontPointsDeque.push_back(cubicCurves.back().anchor2);
	}


}

void CurveLineCubic::clear()
{
	pointsDeque.clear();
	quadraticCurvesDeque.clear();
	cubicCurves.clear();
	frontPointsDeque.clear();
}


Vec2f CurveLineCubic::calcQuadratic(Vec2f p0, Vec2f p1, Vec2f p2, float t)
{
	return Vec2f((1 - t) * (1 - t) * p0.x + 2 * (1 - t) * t * p1.x + t * t * p2.x,
		(1 - t) * (1 - t) * p0.y + 2 * (1 - t) * t * p1.y + t * t * p2.y);

}

void CurveLineCubic::drawCurve(CubicCurve *c)
{
}

void CurveLineCubic::draw()
{


	ci::app::console() << "Cubic Curve List size: " << cubicCurves.size() << std::endl;

	gl::lineWidth(5.0f);

	//gl::color( ColorA( 0, 1, 0, 0.5f ) );
	gl::color(Color(1, 0, 1));
	for (std::vector<CubicCurve>::const_iterator it = cubicCurves.begin(); it != cubicCurves.end(); ++it)
	{
		Path2d	path;

		path.moveTo(it->anchor1);
		path.curveTo(it->control1, it->control2, it->anchor2);

		gl::draw(path);
	}

	gl::lineWidth(5.0f);
	gl::begin(GL_LINE_STRIP);
	for (std::deque<Vec2f>::const_iterator it = frontPointsDeque.begin(); it != frontPointsDeque.end(); ++it)
	{
		gl::vertex(it->x, it->y);
	}
	gl::end();
	
}