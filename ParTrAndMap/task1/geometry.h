#pragma once

#include "stdafx.h"

#include "cv.h"
#include "cxcore.h"
#include "highgui.h"

#include <stdlib.h>
#include <stdio.h>

namespace PTAM {
	class Geometry
	{
		public:
			static void findRt(std::vector<cv::Point2f> points1, std::vector<cv::Point2f> points2, cv::Mat K, cv::Mat &R, cv::Mat &t);
			static std::vector<cv::Point3f> triangulatePoints(std::vector<cv::Point2f> points1, std::vector<cv::Point2f> points2
					, cv::Mat K, cv::Mat R, cv::Mat t);

		private:
			static bool rightSolution(std::vector<double> x1, std::vector<double> x2, std::vector<double> t);
			static bool chooseE(cv::Mat u, cv::Mat W, cv::Mat vt, int direct, std::vector<cv::Point2f> points1, std::vector<cv::Point2f> points2
				, cv::Mat K, cv::Mat &R, cv::Mat &t);
			static cv::Mat findE(std::vector<cv::Point2f> points1, std::vector<cv::Point2f> points2, cv::Mat K);
			static cv::Mat composeProjMatrix(cv::Mat K, cv::Mat R, cv::Mat t);
			static cv::Point3f triangulateOnePoint(cv::Mat P1, cv::Mat P2, cv::Point2f proj1, cv::Point2f proj2);
	};
}