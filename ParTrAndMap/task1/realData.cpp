#include "stdafx.h"

#include <opencv2/opencv.hpp> 
#include "cxcore.h"
#include "highgui.h"
#include <fstream>
#include <iostream>
#include "realData.h"
#include "geometry.h"

#include <stdlib.h>
#include <stdio.h>

using namespace cv;
using namespace std;
using namespace PTAM;

void RealData::initFrames(vector<vector<Point2d>> &frames, string filename)
{
	fstream tracesFile;
	tracesFile.open(filename);

	int frameNumber = 0;
	int currentFrame = 0;
	vector<Point2d> frame(0);

	while (!tracesFile.eof()) {
		tracesFile >> frameNumber;
		if (frameNumber > currentFrame) {
			frames.push_back(frame);
			frame.clear();
			currentFrame = frameNumber;
		}
		double x = 0.0;
		double y = 0.0;
		double coord = 0.0;
		double pointNumber;
		
		tracesFile >> pointNumber >> coord >> x;
		tracesFile >> frameNumber >> pointNumber >> coord >> y;
		frame.push_back(Point2d(x, y));
	}
}

void RealData::findDataFromTraces(vector<Mat_<double>> &Rs, vector<Mat_<double>> &ts, vector<vector<Point2d>> &frames)
{
	Mat K = (Mat_<double>(3, 3) << 313.1852, 0, 217.9243, 0, 312.6343, 127.7195, 0, 0, 1);

	for (int i = 0; i < frames.size() - 1; i++) {
		Mat R = Mat_<double>(3, 3);
		Mat t = Mat_<double>(3, 1);
		Geometry::findRt(frames[i], frames[i + 1], K, R, t);
		Rs.push_back(R);
		ts.push_back(t);
	}
}