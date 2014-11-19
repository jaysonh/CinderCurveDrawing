//
//  MouseVec2f.h
//  DrawingToolkit
//
//  Created by Engage on 12/11/2014.
//
//

#ifndef __DrawingToolkit__MouseVec2f__
#define __DrawingToolkit__MouseVec2f__

#include <stdio.h>
#include "cinder/app/AppNative.h"

using namespace ci;


class TouchVec2f
{
public:
    TouchVec2f();
    TouchVec2f(Vec2f p_);
    TouchVec2f(Vec2f p_, float v_);
    float v;
    Vec2f p;
    
};
#endif /* defined(__DrawingToolkit__MouseVec2f__) */
