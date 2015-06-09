#pragma once

#include "stdafx.h"

#include <opencv2/opencv.hpp> 
#include <opencv\cxcore.hpp>
#include <opencv\highgui.h>

#include <stdlib.h>
#include <stdio.h>

namespace PTAM {
	class SBA
	{
	public:
		SBA(void);
		~SBA(void);
		std::vector<cv::Mat> getRotMatr();
		std::vector<cv::Mat> getTVect();
		std::vector<cv::Point3d> getInitPoints();
		std::vector<std::vector<cv::Point2d>> getProjPoints();
		std::vector<cv::Mat> getK();
		void setInitPoints(std::vector<cv::Point3d> initPoints);
		void estimate();
		void addNewFrameInfo(std::vector<cv::Point2d> projPoints, cv::Mat K, cv::Mat R, cv::Mat t);

	private:
		std::vector<cv::Point3d> mInitPoints;
		std::vector<std::vector<cv::Point2d>> mProjPoints;
		std::vector<cv::Mat> mK;
		std::vector<cv::Mat> mR;
		std::vector<cv::Mat> mT;
		std::vector<cv::Mat> mDist;
		std::vector<std::vector<int>> mVisible;
	};
}
