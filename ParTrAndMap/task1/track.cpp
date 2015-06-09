#include "stdafx.h"

#include "track.h"
#include "point.h"
#include "frame.h"

#include <opencv2/opencv.hpp> 
#include "cxcore.h"
#include "highgui.h"

#include <list>
#include <vector>

using namespace std;
using namespace PTAM;
using namespace cv;


Track::Track()
{
}


Track::~Track(void)
{
}

void Track::addFrame(vector<Point2d> points) 
{
	Frame frm;
	frm.addPoints(points, *this);
	mPoints.push_back(frm.points());
}

void Track::initRecPoints(vector<Point3d> recPoints)
{
	mRecPoints = recPoints;
}

vector<Point3d> Track::recPoints()
{
	return mRecPoints;
}