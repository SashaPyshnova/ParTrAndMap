// task1.cpp: определяет точку входа для консольного приложения.
//

#include "stdafx.h"
#include "cv.h"
#include "cxcore.h"
#include "highgui.h"

#include "model.h"
#include "realData.h"

using namespace std;
using namespace cv;
using namespace PTAM;

int _tmain(int argc, _TCHAR* argv[])
{
//	Model::findRightRAndt();

	vector<Mat_<double>> Rs(0);
	vector<Mat_<double>> ts(0);

	RealData::findDataFromTraces(Rs, ts);
	return 0;
}

