#pragma once

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

class SaveImageParam{
  public:
    string filename;
    int numberOfFrames;
	bool png;
	int counter;

	SaveImageParam(_In_ HWND hwndDlg);
	void addFrame(Mat m_depthMat);

    vector<Mat> frames;
};

string getText( HWND hwnd);
bool getSelectedItem(HWND combobox);
void saveAction(SaveImageParam * params);
void saveJPG(SaveImageParam * params);