#pragma once

#include "stdafx.h"

#include <opencv2/opencv.hpp> 
#include "cxcore.h"
#include "highgui.h"

#include <stdlib.h>
#include <stdio.h>

namespace PTAM {
	class RealData
	{
		public:
			static void findDataFromTraces(std::vector<cv::Mat_<double>> &Rs, std::vector<cv::Mat_<double>> &ts
					, std::vector<std::vector<cv::Point2d>> &frames);
			static void initFrames(std::vector<std::vector<cv::Point2d>> &frames, std::string fileName);

	};
}