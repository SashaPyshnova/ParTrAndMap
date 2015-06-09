#include "stdafx.h"

#include "point.h"

#include <opencv2/opencv.hpp> 
#include "cxcore.h"
#include "highgui.h"

using namespace std;
using namespace PTAM;
using namespace cv;

PTAM::Point::Point(Point2d coordinates
					, Frame& frame
					, Track& track) :
					mCoordinates(coordinates)
					, mFrame(&frame)
					, mTrack(&track)
{

}

