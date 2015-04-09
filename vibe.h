#include <math.h>
#include <cstdlib>
#include <iostream>
#include <ctime>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>

using namespace cv;

class VibeParams{
  public:
   uchar nbSamples;                   
   uchar reqMatches;                   
   uchar radius;                      
   uchar subsamplingFactor; 

   VibeParams();
   VibeParams(uchar nbSamples,uchar reqMatches,uchar radius,uchar subsamplingFactor);

   int getRand();
   bool decideUpdate();
};

class DepthImage{
  public:
	  int height;
	  int width;
	  Mat * data;
      DepthImage(Mat * img);
	  void set(int x,int y,uchar value);
	  uchar get(int x,int y);
	  bool empty(int x,int y);
	  bool inLimitX(double x);
	  bool inLimitY(double y);

	  DepthImage * clone();
  private:
	  uchar*  pointer;
	  int index(int x,int y);
};

class BackgroundModel{
  public:
	int width ; 
    int height;
	int nbSamples; 
	uchar *** samples;
	BackgroundModel(DepthImage * depthImage,VibeParams * vibeParams);
	int compare(int x,int y,uchar point ,VibeParams * vibeParams);
	void update(int x,int y,uchar point ,VibeParams * vibeParams);
	void updateNeighbor(int x,int y,uchar point ,VibeParams * vibeParams);

  private:
    static int nearX[8] ;
    static int nearY[8] ;
    void initSamples(DepthImage * depthImage);
};

extern void  set(int x,int y,uchar value,Mat * image);
extern uchar get(int x,int y,Mat * image);
extern void vibe(Mat * image,VibeParams * params);
extern void connectedCommponents(DepthImage * dimage);
extern void init(int **  table,int height,int width);
extern int markComponent(int x,int y,int componentNumber,int ** relation,DepthImage * dimage,int iter);
extern void clean(int maxComponent,int ** relation,DepthImage * dimage);
