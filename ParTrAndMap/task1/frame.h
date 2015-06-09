#pragma once
#include "stdafx.h"

#include <opencv2/opencv.hpp> 
#include "cxcore.h"
#include "highgui.h"

#include <stdlib.h>
#include <stdio.h>
#include <list>

#include "track.h"

namespace PTAM {
	class Point;
	class Track;
	class Frame
	{
	public:
			Frame();
			void addPoints(std::vector<cv::Point2d> points, Track &track);
			std::list<Point*> points();

	private:
			std::list<Point*> mPoints;	
	};
}

