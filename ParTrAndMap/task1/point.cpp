#include "stdafx.h"

#include "point.h"

#include "cv.h"
#include "cxcore.h"
#include "highgui.h"

using namespace std;
using namespace PTAM;
using namespace cv;

PTAM::Point::Point(Point2f coordinates
					, Frame& frame
					, Track& track) :
					mCoordinates(coordinates)
					, mFrame(&frame)
					, mTrack(&track)
{

}

