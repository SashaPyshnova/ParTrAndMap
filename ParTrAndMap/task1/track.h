#pragma once
#include "stdafx.h"

#include <opencv2/opencv.hpp> 
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
			void addFrame(std::vector<cv::Point2d> points);
			void initRecPoints(std::vector<cv::Point3d> recPoints);
			std::vector<cv::Point3d> recPoints();

	private:
			std::list<std::list<Point*>> mPoints;
			std::vector<cv::Point3d> mRecPoints;
	};
}

