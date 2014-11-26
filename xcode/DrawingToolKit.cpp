//
//  DrawingToolKit.cpp
//  DrawingToolkitFinal
//
//  Created by Engage on 14/11/2014.
//
//

#include "DrawingToolKit.h"


DrawingToolKit::DrawingToolKit()
{
    _r = 0.0f;
    _g = 1.0f;
    _b = 0.0f;
}

void DrawingToolKit::init(int width, int height)
{
    gl::Fbo::Format format;
    format.setSamples(8);
    
    _fbo = gl::Fbo(width, height, format);
    
}

void DrawingToolKit::setSize(int width, int height)
{
    init(width,height);
}

void DrawingToolKit::setTool(DrawToolType toolType)
{
    _currentTool = toolType;
}

void DrawingToolKit::setColour(float r, float g, float b)
{
    _r = r;
    _g = g;
    _b = b;
    
}

void DrawingToolKit::update()
{
    if(_curveList.size() > 0)
    {
        // Update only the last curve in the list since all the others have finished drawing
        _curveList.back().update();
        
    }
    refreshFbo();
}

void DrawingToolKit::refreshFbo()
{
    _fbo.bindFramebuffer();
    gl::clear(ColorA(0.0f,0.0f,0.0f,0.0f));
    
    // Draw our curves to the fbo
    int numSections=0;
    for (std::vector<Curve>::iterator it = _curveList.begin(); it != _curveList.end() ; ++it)
    {
        (*it).draw();
        
        numSections += (*it).getNumSections();
    }
    
    _fbo.unbindFramebuffer();
    ci::app::console() << "Num sections: " << numSections << std::endl;
}

void DrawingToolKit::draw()
{
    cinder::gl::enableAlphaBlending();
    // Setup texture
    gl::color(Color(1.0f,1.0f,1.0f));
    gl::setMatricesWindow(getWindowSize());
    gl::Texture tex(_fbo.getTexture());
    
    // Textures are inverted in y direction by default
    tex.setFlipped(true);
    
    // Draw Texture to the screen
    gl::draw(tex, Rectf(0,0,_fbo.getWidth(),_fbo.getHeight()));

}

gl::Texture DrawingToolKit::getTexture()
{
    return _fbo.getTexture();
}

void DrawingToolKit::clear()
{
    
    _curveList.clear();
    refreshFbo();
    
}

void DrawingToolKit::beginTouch(Vec2f pos, int id)
{
    Curve curve = Curve(_r,_g,_b, _currentTool);
    curve.mouseDown(pos);
    _curveList.push_back(curve);
}

void DrawingToolKit::moveTouch(Vec2f pos, int id)
{
    if(_curveList.size() > 0)
        _curveList.back().mouseDrag(pos,2.0f);
}

void DrawingToolKit::endTouch(Vec2f pos, int id)
{
    if(_curveList.size() > 0)
        _curveList.back().mouseUp(pos);
    
}