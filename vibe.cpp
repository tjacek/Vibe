#include "vibe.h"

int metric(uchar x,uchar y){
	int x0 = (int) x;
	int y0 = (int) y;
	return abs(x-y);
}

VibeParams::VibeParams(){
    this->nbSamples = 10;                  
    this->reqMatches = 1;                   
    this->radius = 2;                     
    this->subsamplingFactor = 16;  
}

VibeParams::VibeParams(int nbSamples,int reqMatches,int radius,int subsamplingFactor){
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
		if(metric(point,sample) < vibeParams->radius){
		//if(sample==point){
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
	if(x0<0){
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
	}
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
}

void  DepthImage::set(int x,int y,uchar value){
    uchar* p=data->ptr<uchar>(0);
	int i=index(x,y);
	p[i] =value;
	p[i+1] =value;
	p[i+2] =value;
}

uchar  DepthImage::get(int x,int y){
    uchar* p=data->ptr<uchar>(0);
	return p[index(x,y)];
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
	for(int i=0;i<dimage->height;i++){
		for(int j=0;j<dimage->width;j++){
			//*uchar point=dimage->get(i,j);
			//int count=model->compare(i,j,point,params);
			//bool isInBackground=  (count >= params->reqMatches);
			/*if(isInBackground){
				if(params->decideUpdate()){
				  model->update(i,j,point ,params);
				}
				if(params->decideUpdate()){
				  model->updateNeighbor(i,j,point ,params);
				}
                dimage->set(i,j,255);
		    }else{
			    //set(i,j,saturate_cast<uchar>(255),image);
			}*/
	   }
	}
}