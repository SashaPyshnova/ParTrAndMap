#include "stdafx.h"

#include "frame.h"
#include "point.h"
#include "track.h"

#include "cv.h"
#include "cxcore.h"
#include "highgui.h"

#include <list>
#include <vector>

using namespace std;
using namespace PTAM;
using namespace cv;

Frame::Frame()
{
}

void Frame::addPoints(vector<Point2f> points, Track &track)
{
	for (int i = 0; i < points.size(); i++) {
		mPoints.push_back(new Point(points.at(i), *this, track));
	}
}

list<PTAM::Point*> Frame::points()
{
	return mPoints;	
}