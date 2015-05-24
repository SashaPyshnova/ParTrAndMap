#pragma once
#include "stdafx.h"

#include "cv.h"
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
			Point(cv::Point2f coordinates
					, Frame& frame
					, Track& track);

	private:
			cv::Point2f mCoordinates;
			Frame* mFrame;
			Track* mTrack;
	};
}

