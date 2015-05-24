// task1.cpp: определяет точку входа для консольного приложения.
//

#include "stdafx.h"
#include "cv.h"
#include "cxcore.h"
#include "highgui.h"

#include "model.h"
#include "realData.h"
#include "geometry.h"
#include "model.h"

#include "track.h"

using namespace std;
using namespace cv;
using namespace PTAM;

int _tmain(int argc, _TCHAR* argv[])
{
	//Model::findRightRAndt();

	/*vector<vector<Point2f>> frames;
	RealData::initFrames(frames);

	Mat K = (Mat_<double>(3, 3) << 313.1852, 0, 217.9243, 0, 312.6343, 127.7195, 0, 0, 1);
	Mat R = Mat_<double>(3, 3);
	Mat t = Mat_<double>(3, 1);

	Geometry::findRt(frames.at(0), frames.at(1), K, R, t);
	cout << R << "\n" << t << "\n";

	vector<Point3f> initPointsRe = Geometry::triangulatePoints(frames.at(1), frames.at(2), K, R, t);
	Model::writeInitPointsInFile(initPointsRe, "tracesRe2.txt");
	Mat R3;
	Mat t3;
	Geometry::findRtUsingInitPoints(initPointsRe, frames.at(3), R3, t3, K);
	cout << R3 << "\n" << t3 << "\n"; */

	Track trc;
	vector<Point2f> vc;
	vc.push_back(Point2f(1, 1));
	vc.push_back(Point2f(2, 2));
	vc.push_back(Point2f(3, 3));
	vc.push_back(Point2f(4, 4));
	vc.push_back(Point2f(5, 5));
	vc.push_back(Point2f(6, 6));
	trc.addFrame(vc);
	return 0;
}

