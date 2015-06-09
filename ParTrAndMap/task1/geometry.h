#pragma once

#include "stdafx.h"

#include <opencv2/opencv.hpp> 
#include "cxcore.h"
#include "highgui.h"

#include <stdlib.h>
#include <stdio.h>

namespace PTAM {
	class Geometry
	{
	public:
			static void findRt(std::vector<cv::Point2d> &points1, std::vector<cv::Point2d> &points2, cv::Mat K, cv::Mat &R, cv::Mat &t);
			static std::vector<cv::Point3d> triangulatePoints(std::vector<cv::Point2d> &points1, std::vector<cv::Point2d> &points2
					, cv::Mat K, cv::Mat &R, cv::Mat &t);
			static void findRtUsingInitPoints(std::vector<cv::Point3d> &initPoints
					, std::vector<cv::Point2d> &pointsProj, cv::Mat &R, cv::Mat &t, cv::Mat K);

	private:
			static bool rightSolution(std::vector<double> &x1, std::vector<double> &x2, std::vector<double> &t);
			static bool chooseE(cv::Mat u, cv::Mat W, cv::Mat vt, int direct, std::vector<cv::Point2d> &points1, std::vector<cv::Point2d> &points2
				, cv::Mat K, cv::Mat &R, cv::Mat &t, int index);
			static cv::Mat findE(std::vector<cv::Point2d> &points1, std::vector<cv::Point2d> &points2, cv::Mat K, cv::Mat &wrongPoints);
			static cv::Mat composeProjMatrix(cv::Mat K, cv::Mat R, cv::Mat t);
			static cv::Point3d triangulateOnePoint(cv::Mat P1, cv::Mat P2, cv::Point2d proj1, cv::Point2d proj2);
			static void setParam(cv::Mat u, cv::Mat W, cv::Mat vt, int direct, cv::Mat &R, cv::Mat &t);
	};
}