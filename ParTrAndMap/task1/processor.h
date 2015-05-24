#pragma once

#include "stdafx.h"

#include "cv.h"
#include "cxcore.h"
#include "highgui.h"

#include <stdlib.h>
#include <stdio.h>

#include "track.h"

namespace PTAM {
	class Processor
	{
	public:
		Processor();
		~Processor(void);
		std::vector<cv::Point3f> recTrajectory(std::vector<std::vector<cv::Point2f>> pointsProj);

	private:
		void initMap(std::vector<cv::Point2f> frame1, std::vector<cv::Point2f> frame2
				, std::vector<cv::Point3f> &recTrajectory);
		void defineCameraPosition(std::vector<cv::Point2f> frame, std::vector<cv::Point3f> &recTrajectory);
		enum State {
			MapInitialization
			, CameraPosDef
		};

		State mState;
		Track mTrack;
	};
}

