#include "vibe.h"
#include <list>
#define _USE_MATH_DEFINES
#include <math.h>

#define Dataserie std::list<float>

extern float standardDeviation(std::list<float> * data);

class MyPoint{
  public:
    float x;
    float y;
    float z;
    MyPoint(float x,float y,float z);
	MyPoint();
	void normalize(float normConst);
};

class MyCloud{
  public:
	std::list<MyPoint*> points;
	void depthimageToCloud(DepthImage * image);
	MyPoint * getExtremePoint(bool * f(MyPoint* x,MyPoint* y));
};