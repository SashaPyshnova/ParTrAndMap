#pragma once

#include "stdafx.h"

#include <opencv2/opencv.hpp> 
#include "cxcore.h"
#include "highgui.h"

#include <stdlib.h>
#include <stdio.h>

namespace PTAM {
	class Model
	{
		public:
			static void findRightRAndt();
			static void writeInitPointsInFile(std::vector<cv::Point3d> initPoints, std::string fileName);

		private:
			static void SevenPointsMethod();
			static void generatePoints(std::vector<cv::Point2d> &points1, std::vector<cv::Point2d> &points2, std::vector<cv::Point3d> &initPoints);
			static void writeProjPointsInFile(std::vector<cv::Point2d> points1, std::vector<cv::Point2d> points2);
			static void readFromFile(std::vector<std::vector<cv::Point2d>> &projPoints);
	};
}