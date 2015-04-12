#include "SaveImage.h"

SaveImageParam::SaveImageParam(_In_ HWND hwndDlg){
	HWND hWndEdit= GetDlgItem(hwndDlg, ID_FILENAME_EDIT);
	this->filename=getText(hWndEdit);
	HWND frameEdit= GetDlgItem(hwndDlg, ID_FRAMES_EDIT);
	string strFrames=getText(frameEdit);
	numberOfFrames=atoi(strFrames.c_str());
    HWND combobox= GetDlgItem(hwndDlg, ID_COMBOBOX);
	this->png=getSelectedItem(combobox);
	counter=numberOfFrames;
}

void SaveImageParam::addFrame(Mat m_depthMat){
  Mat newFrame=m_depthMat.clone();
  frames.push_back(newFrame);
}

void saveAction(SaveImageParam * params){
   if(params->png || 1==1){
	   saveJPG(params);
   }else{
	   //saveBinary(params);
   }
}

void saveJPG(SaveImageParam * params){
  CreateDirectory((LPCWSTR)params->filename.c_str(), NULL);
  string dir=""; 
  dir+= params->filename + "/frame";
  for(int i=0;i<params->numberOfFrames;i++){
	  std::ostringstream ss;
      ss << i;
      string str = ss.str();
	  string path=dir+str+".jpg";
	  Mat image=params->frames.at(i);
	  imwrite(path, image );
  }
}

bool getSelectedItem(HWND combobox){
  int itemIndex = SendMessage((HWND) combobox, (UINT) CB_GETCURSEL, (WPARAM) 0, (LPARAM) 0);
  if(itemIndex==1){
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
    std::wstring wstr(buffer);
    delete[] buffer;

    std::string s( wstr.begin(), wstr.end() );
    return s;
}