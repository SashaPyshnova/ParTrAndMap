#pragma once
#include "stdafx.h"

#include "cv.h"
#include "cxcore.h"
#include "highgui.h"

#include <stdlib.h>
#include <stdio.h>
#include <list>

namespace PTAM {
	class Point;
	class Track
	{
	public:
			Track();
			~Track(void);
			void addFrame(std::vector<cv::Point2f> points);

	private:
			std::list<std::list<Point*>> mPoints;
			std::list<cv::Point3f> mRecPoints;
	};
}

