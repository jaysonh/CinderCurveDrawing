//
//  DrawingToolKit.h
//  DrawingToolkitFinal
//
//  Created by Engage on 14/11/2014.
//
//

#ifndef DRAWTOOLKIT_H
#define DRAWTOOLKIT_H

#include <stdio.h>
#include "cinder/app/AppBasic.h"
#include "cinder/app/AppNative.h"
#include "cinder/gl/Texture.h"
#include "cinder/gl/Fbo.h"
#include "cinder/gl/gl.h"

#include "Curve.h"


using namespace ci;
using namespace ci::app;

class DrawingToolKit
{
public:
    DrawingToolKit();
    
    void init(int width, int height);
    void setSize(int width, int height);
    void setTool(DrawToolType toolType);
    void setColour(float r, float g, float b);
    void update();
    void draw();
    
    gl::Texture getTexture();
    void clear();
    
    void beginTouch(Vec2f pos, int id);
    void moveTouch(Vec2f pos, int id);
    void endTouch(Vec2f pos, int id);
    
private:
    void refreshFbo();
    
    float           _r;
    float           _g;
    float           _b;
    DrawToolType    _currentTool;
    gl::Fbo         _fbo;
    
    vector <Curve>  _curveList;
    
};
#endif 
