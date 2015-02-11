#include "vibe.h"

uchar inline metric(uchar  x,uchar y){
	
	if(x<y){
		return y-x;
	}
	return x-y;
}

VibeParams::VibeParams(){
    this->nbSamples = 10;                  
    this->reqMatches = 1;                   
    this->radius = 2;                     
    this->subsamplingFactor = 16;  
}

VibeParams::VibeParams(uchar nbSamples,uchar reqMatches,uchar radius,uchar subsamplingFactor){
    this->nbSamples = nbSamples;                  
    this->reqMatches = reqMatches;                   
    this->radius = radius;                     
    this->subsamplingFactor = radius;
	std::srand((unsigned int) std::time(0));
  }

int VibeParams::getRand(){
	return std::rand() % subsamplingFactor;
}

bool VibeParams::decideUpdate(){
	 return getRand()==0;
}

int BackgroundModel::nearX[8]={-1,0,1,-1, 1,-1, 0, 1};
int BackgroundModel::nearY[8]={ 1,1,1, 0, 0,-1,-1,-1};

BackgroundModel::BackgroundModel(DepthImage * depthImage,VibeParams * vibeParams){
	this->width=depthImage->width;
	this->height=depthImage->height;
    this->nbSamples=vibeParams->nbSamples;
	initSamples(depthImage);
		
}

int BackgroundModel::compare(int x,int y,uchar point ,VibeParams * vibeParams){
	int count = 0,index=0;
	while ((count < vibeParams->reqMatches) && (index < nbSamples)){
		uchar sample = samples[x][y][index];
		uchar distance=metric(point,sample);
		if(distance < vibeParams->radius){
			count++;
		}
		index++;
	}
	return count;
}

void BackgroundModel::updateNeighbor(int x,int y,uchar point ,VibeParams * vibeParams){
    int randn= rand() % 9;
	int x0 = BackgroundModel::nearX[randn];
	int y0 = BackgroundModel::nearY[randn];

	x0+= x;
	y0+= y;
	/*if(x0<0){
		x0=0;
	}
	if(y0<0){
		y0=0;
	}
	if(x0>= this->height){
		x0=0;
	}
	if(y0>= this->width){
		y0=0;
	}*/
	samples[x0][y0][vibeParams->getRand()]=point;
}

void BackgroundModel::update(int x,int y,uchar point ,VibeParams * vibeParams){
	samples[x][y][vibeParams->getRand()]=point;
}

void BackgroundModel::initSamples(DepthImage * depthImage ){
    samples=new uchar**[height] ;//[height][nbSamples];
	for(int i=0;i<height;i++){
		samples[i]=new uchar*[width];
		for(int j=0;j<width;j++){
			samples[i][j]=new uchar[nbSamples];
			for(int k=0;k<nbSamples;k++){
				samples[i][j][k]= depthImage->get(i,j);
	        }
	    }
	}
}

DepthImage::DepthImage(Mat * img){
	this->data=img;
	int size = img->cols;
	size*= img->channels();
	size*= (img->rows);
	if(size==480*640*4){
		this->height=480;
		this->width=640;
	}else{
		this->height=240;
		this->width=320;
	}
	pointer=data->ptr<uchar>(0);
}

void  DepthImage::set(int x,int y,uchar value){
	int i=index(x,y);
	pointer[i] =value;
	pointer[i+1] =value;
	pointer[i+2] =value;
}

uchar  DepthImage::get(int x,int y){
	return pointer[index(x,y)];
}

bool DepthImage::empty(int x,int y){
   return 255!=get(x,y);
}

bool  DepthImage::inLimitX(double x){
	int x0=floor(x);
	return 0<=x0 && x0<=this->height;  
}

bool  DepthImage::inLimitY(double y){
	int y0=floor(y);
	return 0<=y0 && y0<=this->height;  
}

DepthImage* DepthImage::clone(){
	Mat m=this->data->clone();
	DepthImage* clonedImage=new DepthImage(&m);
	return clonedImage;
}

int DepthImage::index(int x,int y){
	int index= x*this->width + y;
	index*=4;
	return index;
}

static BackgroundModel * model=NULL;

void vibe(Mat * image,VibeParams * params){
	DepthImage * dimage=new DepthImage(image);
	if(model==NULL){
        model =new  BackgroundModel(dimage,params);
	}
	for(int i=1;i<dimage->height-1;i++){
		for(int j=1;j<dimage->width-1;j++){
			 uchar point=dimage->get(i,j);
			int count=model->compare(i,j,point,params);
			bool isInBackground=  (count >= params->reqMatches);
			if(isInBackground){
				if(params->decideUpdate()){
				  model->update(i,j,point ,params);
				}
				if(params->decideUpdate()){
				  model->updateNeighbor(i,j,point ,params);
				}
                dimage->set(i,j,255);
		    }else{
			}
	   }
	}
}

void connectedCommponents(DepthImage * dimage){
  int ** relation=new int*[dimage->height];
  for(int i=0;i<dimage->height;i++){
    relation[i]=new int[dimage->width];
  }
  init(  relation,dimage->height,dimage->width);
  int maxComponentSize=0;
  int maxComponent=1;
  int currentComponent=1;
  for(int i=1;i<dimage->height-1;i++){
	for(int j=1;j<dimage->width-1;j++){
        if(relation[i][j]==0){
			int sizeOfComponent=markComponent(i,j,currentComponent,relation,dimage);
			if(maxComponentSize<sizeOfComponent ){
				maxComponentSize=sizeOfComponent;
				maxComponent=currentComponent;
			}
			currentComponent++;
		}
	}
  }
  clean(maxComponent,relation,dimage);
}

void init(int **  table,int height,int width){
  for(int i=0;i<height;i++){
	  table[i][0]=-1;
	  table[i][width-1]=-1;
  }
  for(int j=0;j<width;j++){
	  table[0][j]=-1;
	  table[height-1][j]=-1;
  }
  for(int i=1;i<height-1;i++){
    for(int j=1;j<width-1;j++){
	  table[i][j]=0;	 
    }
  }
}
bool checkBounds(int x,int y,int ** relation,DepthImage * dimage){
  if(x<=0){
	return false;
  }
  
  if(y<=0){
	return false;
  }

  if(dimage->height <=x){
	return false;
  }

  if(dimage->width <=y){
	return false;
  }

  return true;
}

int markComponent(int x,int y,int componentNumber,int ** relation,DepthImage * dimage){
   if(checkBounds( x, y, relation,dimage)){
   uchar value=dimage->get(x,y);
   int numberOfPixels=0;
   if(relation[x][y]==0 &value!=255){
     relation[x][y]=componentNumber;
	 numberOfPixels+=markComponent(x  ,y+1,componentNumber,relation, dimage);
	 numberOfPixels+=markComponent(x+1,y  ,componentNumber,relation, dimage);
     numberOfPixels+=markComponent(x+1,y+1,componentNumber,relation, dimage);
	 numberOfPixels+=markComponent(x  ,y-1,componentNumber,relation, dimage);
	 numberOfPixels+=markComponent(x-1,y  ,componentNumber,relation, dimage);
	 numberOfPixels+=markComponent(x-1,y-1,componentNumber,relation, dimage);
     numberOfPixels+=markComponent(x-1,y+1,componentNumber,relation, dimage);
	 numberOfPixels+=markComponent(x+1,y-1,componentNumber,relation, dimage);
	 numberOfPixels+=1;
	 return numberOfPixels;
     }
   }
   return 0 ;
}

void clean(int maxComponent,int ** relation,DepthImage * dimage){
  for(int i=0;i<dimage->height;i++){
	for(int j=0;j<dimage->width;j++){
	  if(relation[i][j]!=maxComponent){
		  dimage->set(i,j,255);
	  }
	}
  }
}
