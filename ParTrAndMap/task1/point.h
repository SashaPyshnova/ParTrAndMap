#pragma once
#include "stdafx.h"

#include <opencv2/opencv.hpp> 
#include "cxcore.h"
#include "highgui.h"

#include <stdlib.h>
#include <stdio.h>

#include "frame.h"
#include "track.h"

namespace PTAM {
	class Point
	{
	public:
			Point(cv::Point2d coordinates
					, Frame& frame
					, Track& track);

	private:
			cv::Point2d mCoordinates;
			Frame* mFrame;
			Track* mTrack;
	};
}

