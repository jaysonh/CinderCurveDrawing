//
//  MouseVec2f.cpp
//  DrawingToolkit
//
//  Created by Engage on 12/11/2014.
//
//

#include "TouchVec2f.h"

TouchVec2f::TouchVec2f()
{
    p = Vec2f(0,0);
    v = 0.0f;
}
TouchVec2f::TouchVec2f(Vec2f p_)
{
    p = p_;
}

TouchVec2f::TouchVec2f(Vec2f p_, float v_)
{
    p = p_;
    v = v_;
}