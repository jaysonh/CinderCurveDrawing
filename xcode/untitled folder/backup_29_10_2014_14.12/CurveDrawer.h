//
//  CurveDrawer.h
//  DrawingToolkit
//
//  Created by Engage on 28/10/2014.
//
//

#ifndef __DrawingToolkit__CurveDrawer__
#define __DrawingToolkit__CurveDrawer__

#include <stdio.h>

#include "cinder/Rand.h"
#include "cinder/Utilities.h"
#include "cinder/gl/Texture.h"
#include "cinder/Text.h"
#include "cinder/Utilities.h"
#include "cinder/ImageIo.h"
#include "cinder/Font.h"
#include "cinder/Vector.h"
#include "cinder/Path2d.h"
#include "cinder/gl/gl.h"
#include "CinderOpenCV.h"
#include "cinder/app/AppNative.h"
#include "CurvesQuadratic.h"

using namespace ci;
using namespace ci::app;
using namespace std;

class CurveDrawer
{
public:
    
    CurveDrawer();
    void mouseDown(MouseEvent event);
    void mouseUp(MouseEvent event);
    void mouseDrag(MouseEvent event, float minDistance);
    void update();
    
    void clear();
    void draw();
    
private:
    CurvesQuadratic *currentCurve;
    
    std::vector <CurvesQuadratic*> curveList;
};

#endif 
