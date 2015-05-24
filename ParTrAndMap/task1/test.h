#pragma once

#include "stdafx.h"

#include "cv.h"
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

	private: 
		std::vector<cv::Point3f> readTrajectory(std::string fileName);
		std::vector<std::vector<cv::Point2f>> createProjections(std::vector<cv::Point3f> trajectory
				, std::vector<cv::Point3f> initPoints);
		std::vector<cv::Point3f> generatePoints(int numberOfPoints);
		void write3fPointsInFile(std::vector<cv::Point3f> initPoints, std::string fileName);
		void write2fPointsInFile(std::vector<std::vector<cv::Point2f>> points, std::string fileName);
	};
}

