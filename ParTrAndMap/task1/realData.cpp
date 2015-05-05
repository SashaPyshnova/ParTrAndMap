#include "stdafx.h"

#include "cv.h"
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

void RealData::initFrames(vector<vector<Point2f>> &frames)
{
	fstream tracesFile;
	tracesFile.open("traces.txt");

	int frameNumber = 0;
	int cornerNumber = 0;

	while (!tracesFile.eof()) { 
		tracesFile >> frameNumber;
		if (tracesFile.eof())
			return;
		tracesFile >> cornerNumber;

		char symbol;
		tracesFile >> symbol;
		double x = 0.0;
		double y = 0.0;
		vector<Point2f> frame(0);
		for (int i = 0; i < cornerNumber; i++) {
			while (symbol != '[')
				tracesFile >> symbol;
			tracesFile >> x;
			while (symbol != ',')
				tracesFile >> symbol;
			tracesFile >> y;
			frame.push_back(Point2f(x, y));
		}
		while (symbol != ']') {
				tracesFile >> symbol;
		}
		frames.push_back(frame);
	}
}

void RealData::findDataFromTraces(vector<Mat_<double>> &Rs, vector<Mat_<double>> &ts)
{
	Mat K = (Mat_<double>(3, 3) << 313.1852, 0, 217.9243, 0, 312.6343, 127.7195, 0, 0, 1);

	vector<vector<Point2f>> frames(0);

	initFrames(frames);
	for (int i = 0; i < frames.size() - 1; i++) {
		Mat R = Mat_<double>(3, 3);
		Mat t = Mat_<double>(3, 1);
		Geometry::findRt(frames[i], frames[i + 1], K, R, t);
		Rs.push_back(R);
		ts.push_back(t);
	}
}