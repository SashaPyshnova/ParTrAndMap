#pragma once

#include "stdafx.h"

#include <opencv2/opencv.hpp> 
#include "cxcore.h"
#include "highgui.h"

#include <stdlib.h>
#include <stdio.h>
#include <vector>

namespace PTAM {
	class Test
	{
	public:
		Test(void);
		~Test(void);
		void findRecTrajectory(std::string fileName);
		void findRecTrajectoryReal(std::string fileName);

	private: 
		std::vector<cv::Point3d> readTrajectory(std::string fileName);
		std::vector<std::vector<cv::Point2d>> createProjections(std::vector<cv::Point3d> trajectory
				, std::vector<cv::Point3d> initPoints);
		std::vector<cv::Point3d> generatePoints(int numberOfPoints);
		void write3fPointsInFile(std::vector<cv::Point3d> initPoints, std::string fileName);
		void write2fPointsInFile(std::vector<std::vector<cv::Point2d>> points, std::string fileName); 
		void readTandR(std::string fileName, std::vector<cv::Point3d> &t, std::vector<cv::Point3d> &R);
		std::vector<std::vector<cv::Point2d>> createProjections(std::vector<cv::Point3d> initPoints,  std::vector<cv::Point3d> t
				, std::vector<cv::Point3d> R, cv::Mat K);
		void writeTrajectory(std::vector<cv::Mat> R, std::vector<cv::Mat> t, std::string fileName);
	};
}

