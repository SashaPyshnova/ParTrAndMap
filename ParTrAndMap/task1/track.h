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
			void initRecPoints(std::vector<cv::Point3f> recPoints);
			std::vector<cv::Point3f> recPoints();

	private:
			std::list<std::list<Point*>> mPoints;
			std::vector<cv::Point3f> mRecPoints;
	};
}

