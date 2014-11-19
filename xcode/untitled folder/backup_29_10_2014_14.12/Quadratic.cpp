#include "Quadratic.h"

Quadratic::Quadratic(const Quadratic &ref)
{
    p1 = ref.p1;
    p2 = ref.p2;
    p3 = ref.p3;
}
Quadratic::Quadratic()
{
    p1 = Vec2f(0,0);
    p2 = Vec2f(0,0);
    p3 = Vec2f(0,0);
}
Quadratic::Quadratic(Vec2f p1_, Vec2f p2_, Vec2f p3_,bool isBeginning, bool isEnd)
{
    if(isBeginning)
    {
        
        p1 = p1_;
        p2 = getMidPoint(p1_, p2_);
        p3 = getMidPoint(p1_, p2_);
    }else if(isEnd)
    {
        
        p1 = getMidPoint(p1_, p2_);
        p2 = p2_;
        p3 = p3_;
        
    }else
    {
        
        p1 = getMidPoint(p1_, p2_);
        p2 = p2_;
        p3 = getMidPoint(p2_, p3_);
        
    }
	//ci::app::console() << "new quad " << p1_ << " " << p2_ << " " << p3_ << std::endl;
}

void Quadratic::draw()
{
	Path2d	path;

	path.moveTo(p1);

	path.quadTo(p2, p3);

	gl::draw(path);
}


Vec2f Quadratic::getMidPoint(Vec2f p1, Vec2f p2)
{
	return Vec2f((p2.x - p1.x) / 2.0 + p1.x, (p2.y - p1.y) / 2.0 + p1.y);
}