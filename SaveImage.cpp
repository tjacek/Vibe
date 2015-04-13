#include "SaveImage.h"

SaveImageParam::SaveImageParam(_In_ HWND hwndDlg){
	HWND hWndEdit= GetDlgItem(hwndDlg, ID_FILENAME_EDIT);
	this->filename=getText(hWndEdit);
	HWND frameEdit= GetDlgItem(hwndDlg, ID_FRAMES_EDIT);
	string strFrames=getText(frameEdit);
	numberOfFrames=atoi(strFrames.c_str());
    HWND combobox = GetDlgItem(hwndDlg, ID_COMBOBOX);
	this->png=getSelectedItem(combobox);
	counter=numberOfFrames;
}

void SaveImageParam::addFrame(Mat m_depthMat){
  Mat newFrame=m_depthMat.clone();
  frames.push_back(newFrame);
}

int SaveImageParam::getCols(){
  Mat firstFrame=frames.at(0);
  return firstFrame.cols;
}

int SaveImageParam::getRows(){
  Mat firstFrame=frames.at(0);
  return firstFrame.rows;
}

void saveAction(SaveImageParam * params){
   if(params->png){
	   saveJPG(params);
   }else{
	  saveBinary(params);
   }
}

void saveJPG(SaveImageParam * params){
  CreateDirectory( (LPCTSTR)params->filename.c_str(), NULL);

  string dir(params->filename.begin(),params->filename.end()); 
  dir+="/frame";
  for(int i=0;i<params->numberOfFrames;i++){
	  std::ostringstream ss;
      ss << i;
      string str = ss.str();
	  string path=dir+str+".jpg";
	  Mat image=params->frames.at(i);
	  imwrite(path, image );
  }
}

void saveBinary(SaveImageParam * params){
  string filename=params->filename+".bin";
  FILE * fp = fopen(filename.c_str(), "wb");
  writeActionHeader(fp, params->frames.size(), params->getCols(), params->getRows());
  for(int i=0;i<params->numberOfFrames;i++){
	writeFrame(fp,params->frames.at(i));
  }
}

void writeActionHeader(FILE * fp  , int nFrames, int nCols, int nRows){
  if(fp == NULL)
		return;
  fwrite(&nFrames, 4, 1, fp); //read 4 bytes 
  fwrite(&nCols, 4, 1, fp);
  fwrite(&nRows, 4, 1, fp);
}

 void writeFrame(FILE * fp, cv::Mat frame){
   int numCols = frame.cols;
   int numRows = frame.rows;
	
	int r,c;
	//for(h=0; h<height; h++) //for each row
	int * tempRow = new int[numCols];
	for(r=0;r<numRows;r++) //one row at a time
	{
		for(c=0; c<numCols; c++) //for each colume
		{
			int temp = (int) (frame.at<uchar>(r,c) );
			tempRow[c] = temp;
		}
		fwrite(tempRow, 4, numCols, fp);
	}
	delete[] tempRow;
	tempRow = NULL;
 }

bool getSelectedItem(HWND combobox){
  int itemIndex = SendMessage((HWND) combobox, (UINT) CB_GETCURSEL, (WPARAM) 0, (LPARAM) 0);
  if(itemIndex==0){
	return true;
  }else{
	return false;
  }
  //TCHAR FormatItems[9][10] =  { TEXT("BINARY"), TEXT("PNG") };	
  //     MessageBox(hwndDlg, (LPCWSTR) ListItem, TEXT("Item Selected"), MB_OK);    
}

string getText( HWND hwnd)
{
    int length = SendMessage(hwnd,WM_GETTEXTLENGTH,0,0);
    if(length == -1)
        return "";
    wchar_t* buffer = new wchar_t[length+1];
    SendMessage(hwnd,WM_GETTEXT,length+1,(LPARAM)buffer);
    std::wstring  wstr(buffer);
	//CW2T pszT(wstr.c_str());
	//MessageBox(hwnd, pszT, TEXT("Item Selected"), MB_OK);

	delete[] buffer;
    std::string s( wstr.begin(), wstr.end() );
    return s;
}