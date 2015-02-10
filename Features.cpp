#include "Features.h"

MyPoint::MyPoint(float x,float y,float z){
  this->x=x;
  this->y=y;
  this->z=z;
}

MyPoint::MyPoint(){
  this->x=x;
  this->y=y;
  this->z=z;
}

void MyPoint::normalize(float normConst){
  this->x/=normConst;
  this->y/=normConst;
  this->z/=normConst;
}

void MyCloud::depthimageToCloud(DepthImage * image){
  for(int i=0;i<image->height;i++){
    for(int j=0;j<image->width;j++){
	   uchar z=image->get(i,j);
       MyPoint * point=new MyPoint(i,j,z);
	   this->points.push_front(point);
    }
  }
}

MyPoint * MyCloud::getExtremePoint(bool * compare(MyPoint* x,MyPoint* y)){
  MyPoint* extremePoint=NULL;
  for (std::list<MyPoint*>::iterator it = points.begin(); it != points.end(); it++){
	MyPoint* currentPoint=*it;
	if(extremePoint!=NULL){
	  if(compare(currentPoint,extremePoint)){
		extremePoint==currentPoint;
	  }
	}else{
		extremePoint==currentPoint;
	}
  }
  return extremePoint;
}

bool maxX(MyPoint * p1,MyPoint * p2){
	if(p1->x < p2->x){
		return true;
	}
	return false;
}

void radialStd(DepthImage * image,MyPoint * centerOfMass){
  double r=0,theta=0;
  double step= M_PI /8;
  while(theta<M_PI){
	 double x_0=centerOfMass->x;
	 double y_0=centerOfMass->y;
	 Dataserie dataserie;
	 while (image->inLimitX(x_0) && image->inLimitY(y_0))
	 {
		 r+=1.0;
		 double detX=r*cos(theta);
		 double detY=r*sin(theta);
		 int x_i=floor(x_0 + detX);
		 int y_i=floor(y_0 + detY);
		 if(image->empty(x_i,x_i)){
	       dataserie.push_front(image->get(x_i,y_i));
	     }
	 }
	standardDeviation(&dataserie);
    theta+=step;
  }
}
