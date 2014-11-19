#ifndef QUADRATIC_H
#define QUADRATIC_H


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


using namespace ci;
using namespace ci::app;
using namespace std;



class Quadratic
{

public:
    Quadratic();
    Quadratic(const Quadratic &ref);
    Quadratic(Vec2f p1_, Vec2f p2_, Vec2f p3_,bool isBeginning=false,bool isEnd=false);
    void draw();
    Vec2f getMidPoint(Vec2f p1, Vec2f p2);
	Vec2f	p1;
	Vec2f	p2;
	Vec2f	p3;

private:

};

#endif




