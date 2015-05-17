#pragma once

#include "stdafx.h"

#include "cv.h"
#include "cxcore.h"
#include "highgui.h"

#include <stdlib.h>
#include <stdio.h>

namespace PTAM {
	class Model
	{
		public:
			static void findRightRAndt();

		private:
			static void SevenPointsMethod();
			static void generatePoints(std::vector<cv::Point2f> &points1, std::vector<cv::Point2f> &points2, std::vector<cv::Point3f> &initPoints);
			static void writeProjPointsInFile(std::vector<cv::Point2f> points1, std::vector<cv::Point2f> points2);
			static void writeInitPointsInFile(std::vector<cv::Point3f> initPoints, std::string fileName);
			static void readFromFile(std::vector<cv::Point2f> &points1, std::vector<cv::Point2f> &points2);
	};
}