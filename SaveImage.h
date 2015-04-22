#pragma once

#include <iostream>
#include <fstream>
#include <Windows.h>
#include <tchar.h>
#include <CommCtrl.h>
#include <string>
#include <sstream>
#include <vector>
#include "time.h"
#include "math.h"
#include <NuiApi.h>
#include "OpenCVHelper.h"
#include "FrameRateTracker.h"
#include <AtlBase.h>
#include <AtlConv.h>
#include "vibe.h"

using std::wstring;

class SaveImageParam{
  public:
    string filename;
    int numberOfFrames;
	bool png;
	int counter;

	SaveImageParam(_In_ HWND hwndDlg);
	void addFrame(Mat m_depthMat);

	int getCols();
	int getRows();
    vector<Mat> frames;
};

string getText( HWND hwnd);
bool getSelectedItem(HWND combobox);
void saveAction(SaveImageParam * params);
void saveJPG(SaveImageParam * params);
void saveBinary(SaveImageParam * params);
void writeActionHeader(FILE *   actionFile, int nFrames, int nCols, int nRows);
void writeFrame(FILE * fp, cv::Mat frame);