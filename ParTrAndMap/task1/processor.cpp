#include "stdafx.h"
#include "processor.h"

#include "cv.h"
#include "cxcore.h"
#include "highgui.h"

#include <stdlib.h>
#include <stdio.h>

#include "geometry.h"
#include "track.h"

using namespace cv;
using namespace std;
using namespace PTAM;

Processor::Processor()
{
	mState = MapInitialization;
	mTrack = Track();
}

Processor::~Processor(void)
{
}

void Processor::initMap(vector<Point2f> frame1, vector<Point2f> frame2, vector<Point3f> &recTrajectory)
{
	Mat K = (Mat_<double>(3, 3) << 1, 0, 50, 0, 1, 50, 0, 0, 1); //из ген точек
	Mat R;
	Mat t;
	Geometry::findRt(frame1, frame2, K, R, t); // ƒобавить R, t в траекторию
	cout << R << "\n";
	cout << t << "\n";
	vector<Point3f> initPoints = Geometry::triangulatePoints(frame1, frame2, K, R, t);
	mTrack.initRecPoints(initPoints);
	recTrajectory.push_back(Point3f(t));
}

void Processor::defineCameraPosition(vector<Point2f> frame, vector<Point3f> &recTrajectory)
{
	Mat K = (Mat_<double>(3, 3) << 1, 0, 50, 0, 1, 50, 0, 0, 1); //из ген точек
	Mat R;
	Mat t;
	Geometry::findRtUsingInitPoints(mTrack.recPoints(), frame, R, t, K); // ƒобавить R, t в траекторию
	cout << R << "\n";
	cout << t << "\n";
	recTrajectory.push_back(Point3f(t));
}

vector<Point3f> Processor::recTrajectory(vector<vector<Point2f>> pointsProj)
{
	vector<Point3f> recTrajectory;
	while (!pointsProj.empty()) {
		vector<Point2f> frame = pointsProj.front();
		pointsProj.erase(pointsProj.begin());

		switch(mState) 
		{
		case MapInitialization:
			{	
				mTrack.addFrame(frame);
				recTrajectory.push_back(Point3f(0.0, 0.0, 0.0));
				//добавить ожидание
				vector<Point2f> frame2 = pointsProj.front();
				pointsProj.erase(pointsProj.begin());
				initMap(frame, frame2, recTrajectory);

				mState = CameraPosDef;
				break;
			}
		case CameraPosDef:
			{
				defineCameraPosition(frame, recTrajectory);
				break;
			}
		}
	}
	return recTrajectory;
}