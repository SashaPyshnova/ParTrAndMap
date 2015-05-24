#pragma once

#include "stdafx.h"

#include "cv.h"
#include "cxcore.h"
#include "highgui.h"

#include <stdlib.h>
#include <stdio.h>

namespace PTAM {
	class RealData
	{
		public:
			static void findDataFromTraces(std::vector<cv::Mat_<double>> &Rs, std::vector<cv::Mat_<double>> &ts
					, std::vector<std::vector<cv::Point2f>> &frames);
			static void initFrames(std::vector<std::vector<cv::Point2f>> &frames);

	};
}