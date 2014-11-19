//
//  CurveDrawer.cpp
//  DrawingToolkit
//
//  Created by Engage on 28/10/2014.
//
//

#include "CurveDrawer.h"

CurveDrawer::CurveDrawer()
{
    curveList.push_back(new CurvesQuadratic());
    
    currentCurve = new CurvesQuadratic();
}
void CurveDrawer::mouseDown(MouseEvent event)
{
    curveList.push_back(new CurvesQuadratic());
    curveList[curveList.size()-1]->mouseDown(event);
    currentCurve = new CurvesQuadratic();
    currentCurve->mouseDown(event);
}
void CurveDrawer::mouseUp(MouseEvent event)
{
    currentCurve->mouseUp(event);
    
    curveList[curveList.size()-1]->mouseUp(event);
    
    CurvesQuadratic * newCurve = new CurvesQuadratic();
    newCurve->mouseUp(event);
     curveList.push_back(newCurve);
}
void CurveDrawer::mouseDrag(MouseEvent event, float minDistance)
{
    curveList[curveList.size()-1]->mouseUp(event);
    currentCurve->mouseDrag(event,minDistance);
}

void CurveDrawer::update()
{
    
}

void CurveDrawer::clear()
{
    
    currentCurve = new CurvesQuadratic();
}
void CurveDrawer::draw()
{
    gl::color(Color(1.0, 0.0, 0.0));
    currentCurve->update();
    currentCurve->draw();
    ci::app::console() << "num curves: " <<curveList.size() << endl;
    for (std::vector<CurvesQuadratic*>::const_iterator it = curveList.begin(); it != curveList.end(); ++it)
    {
        (*it)->update();
        (*it)->draw();
    }
}