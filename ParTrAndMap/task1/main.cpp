// task1.cpp: определяет точку входа для консольного приложения.
//

#include "stdafx.h"
#include "cv.h"
#include "cxcore.h"
#include "highgui.h"

#include "test.h"

using namespace std;
using namespace cv;
using namespace PTAM;

int _tmain(int argc, _TCHAR* argv[])
{
	Test newTest;
	newTest.findRecTrajectory("trajectory.txt");
	return 0;
}

