#include "stdafx.h"
#include "sba.h"

#include <opencv2\opencv.hpp> 
#include <opencv\cxcore.hpp>
#include <opencv\highgui.h>

#include <stdlib.h>
#include <stdio.h>

#include <vector>

using namespace cv;
using namespace std;
using namespace PTAM;

SBA::SBA(void)
{
}


SBA::~SBA(void)
{
}

void SBA::setInitPoints(vector<Point3d> initPoints)
{
	mInitPoints = initPoints;
}

void SBA::estimate()
{
	cv::LevMarqSparse::bundleAdjust(mInitPoints, mProjPoints, mVisible, mK, mR, mT, mDist
		, TermCriteria(TermCriteria::COUNT+TermCriteria::EPS, 30, DBL_EPSILON));
}

void SBA::addNewFrameInfo(vector<Point2d> projPoints, Mat K, Mat R, Mat t)
{
	mProjPoints.push_back(projPoints);
	mK.push_back(K);
	mR.push_back(R);
	mT.push_back(t);
	mDist.push_back(Mat::zeros(4,1, CV_64FC1));   //

	vector<int> v;
	for (int j = 0; j < projPoints.size(); j++) {
		if (projPoints.at(j) == Point2d(0, 0))
			v.push_back(0);
		else
			v.push_back(1);
	}
	mVisible.push_back(v);
}


vector<Mat> SBA::getRotMatr()
{
	return mR;
}

vector<vector<Point2d>> SBA::getProjPoints()
{
	return mProjPoints;
}

vector<Mat> SBA::getTVect()
{
	return mT;
}

vector<Point3d> SBA::getInitPoints()
{
	return mInitPoints;
}

vector<Mat> SBA::getK()
{
	return mK;
}