#include "stdafx.h"
#include "processor.h"

#include <opencv2\opencv.hpp> 
#include <opencv\cxcore.hpp>
#include <opencv\highgui.h>

#include <stdlib.h>
#include <stdio.h>

#include "geometry.h"
#include "track.h"
#include "sba.h"

using namespace cv;
using namespace std;
using namespace PTAM;

Processor::Processor()
{
	mState = MapInitialization;
	mTrack = Track();
	mSBA = SBA();
	mCount = 0;
}

Processor::~Processor(void)
{
}

bool Processor::isKeyFrame() //нормальный критерий
{
	if (mCount == 50) {
		mCount = 0;
		return true;
	}
	return false;
}

void Processor::initMap(vector<Point2d> frame1, vector<Point2d> frame2
						, vector<Point3d> &recTrajectory, Mat K)
{
	Mat R;
	Mat t;
	Geometry::findRt(frame1, frame2, K, R, t); // ƒобавить R, t в траекторию
	cout << R << "\n";
	cout << t << "\n";

	vector<Point3d> initPoints = Geometry::triangulatePoints(frame1, frame2, K, R, t);
	mTrack.initRecPoints(initPoints);

	mSBA.setInitPoints(initPoints);
	mSBA.addNewFrameInfo(frame2, K, R, t);
	mCount++;

	if (isKeyFrame())
		mSBA.estimate();

	recTrajectory.push_back(Point3d(t));
}

void Processor::defineCameraPosition(vector<Point2d> frame, vector<Point3d> &recTrajectory, Mat K)
{
	Mat R;
	Mat t;
	Geometry::findRtUsingInitPoints(mTrack.recPoints(), frame, R, t, K); // ƒобавить R, t в траекторию
	cout << R << "\n";
	cout << t << "\n";
	recTrajectory.push_back(Point3d(t));
	mSBA.addNewFrameInfo(frame, K, R, t);
	mCount++;
	if (isKeyFrame())
		mSBA.estimate();
}

void Processor::addInitParamToSBA(vector<Point2d> frame, Mat K)
{
	Mat R = Mat::eye(3, 3, CV_64FC1);
	Mat t = Mat::zeros(3, 1, CV_8U);
	mSBA.addNewFrameInfo(frame, K, R, t);
}

void Processor::recTrajectory(vector<vector<Point2d>> pointsProj, Mat K)
{
	vector<Point3d> recTrajectory;
	while (!pointsProj.empty()) {
		vector<Point2d> frame = pointsProj.front();
		pointsProj.erase(pointsProj.begin());

		switch(mState) 
		{
		case MapInitialization:
			{	
				mTrack.addFrame(frame);
				recTrajectory.push_back(Point3d(0.0, 0.0, 0.0));
				//добавить ожидание
				vector<Point2d> frame2 = pointsProj.front();
				pointsProj.erase(pointsProj.begin());

				addInitParamToSBA(frame, K);
				mCount++;
				initMap(frame, frame2, recTrajectory, K);

				mState = CameraPosDef;
				break;
			}
		case CameraPosDef:
			{
				defineCameraPosition(frame, recTrajectory, K);
				break;
			}
		}
	}

	/*for (int i = 0; i < initPoints.size(); i++) {
		Mat prPoint = K * (R * Mat(initPoints.at(i)) + t);
		mProjRecPoints.push_back(Point2d(vector<double>(prPoint.col(0).row(0)).at(0) / vector<double>(prPoint.col(0).row(2)).at(0)
									, vector<double>(prPoint.col(0).row(1)).at(0)/ vector<double>(prPoint.col(0).row(2)).at(0)));
	}*/

	//return recTrajectory;
}

PTAM::Track Processor::getTrack()
{
	return mTrack;
}

vector<vector<Point2d>> Processor::projError()
{
	vector<vector<Point2d>> errors;
	vector<Point3d> initPoints = mSBA.getInitPoints();


	for (int i = 0; i < mSBA.getProjPoints().size(); i++) {
		vector<Point2d> error;
		Mat R = mSBA.getRotMatr().at(i);
		Mat t = mSBA.getTVect().at(i);
		Mat K = mSBA.getK().at(i);
		vector<Point2d> projPoints = mSBA.getProjPoints().at(i);
		for (int j = 0; j < projPoints.size(); j++) {
			if (projPoints.at(j) == Point2d(0, 0))
				error.push_back(Point2d(0, 0));
			else {
				Mat prPoint = K * (R * Mat(initPoints.at(j)) + t);
				Point2d reproj = Point2d(vector<double>(prPoint.col(0).row(0)).at(0) / vector<double>(prPoint.col(0).row(2)).at(0)
									, vector<double>(prPoint.col(0).row(1)).at(0)/ vector<double>(prPoint.col(0).row(2)).at(0));
				error.push_back(projPoints.at(j) - reproj);
			}
		}
		errors.push_back(error);
	}

	return errors;
}

void Processor::getTrajectory(vector<Mat> &R, vector<Mat> &t)
{
	R = mSBA.getRotMatr();
	t = mSBA.getTVect();
}