// task1.cpp: определяет точку входа для консольного приложения.
//

#include "stdafx.h"
#include <opencv2/opencv.hpp> 
#include "cxcore.h"
#include "highgui.h"

#include "test.h"

using namespace std;
using namespace cv;
using namespace PTAM;

int _tmain(int argc, _TCHAR* argv[])
{
	Test newTest;
	newTest.findRecTrajectoryReal("tracesNew.txt");
	return 0;
}
