#pragma once

#include "stdafx.h"

#include <opencv2/opencv.hpp> 
#include <opencv\cxcore.hpp>
#include <opencv\highgui.h>

#include <stdlib.h>
#include <stdio.h>

#include "track.h"
#include "sba.h"

namespace PTAM {
	class Processor
	{
	public:
		Processor();
		~Processor(void);
		void recTrajectory(std::vector<std::vector<cv::Point2d>> pointsProj, cv::Mat K);
		PTAM::Track getTrack();
		std::vector<std::vector<cv::Point2d>> projError();
		void getTrajectory(std::vector<cv::Mat> &R, std::vector<cv::Mat> &t);

	private:
		void initMap(std::vector<cv::Point2d> frame1, std::vector<cv::Point2d> frame2
				, std::vector<cv::Point3d> &recTrajectory, cv::Mat K);
		void defineCameraPosition(std::vector<cv::Point2d> frame, std::vector<cv::Point3d> &recTrajectory
				, cv::Mat K);
		void addInitParamToSBA(std::vector<cv::Point2d> frame, cv::Mat K);
		bool isKeyFrame();

		enum State {
			MapInitialization
			, CameraPosDef
		};

		State mState;
		Track mTrack;
		SBA mSBA;
		int mCount;
	};
}

