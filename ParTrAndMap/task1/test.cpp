#include "stdafx.h"

#include "test.h"
#include "processor.h"
#include "realData.h"

#include <opencv2/opencv.hpp> 
#include "cxcore.h"
#include "highgui.h"

#include <random>
#include <fstream>
#include <iostream>
#include <vector>

using namespace std;
using namespace PTAM;
using namespace cv;

Test::Test(void)
{
}

Test::~Test(void)
{
}

void Test::write3fPointsInFile(vector<Point3d> points, string fileName)
{
	fstream pointsFile(fileName, fstream::trunc | fstream::out);
	pointsFile.trunc;
	for (int i = 0; i < points.size(); i++) {
		pointsFile << points.at(i).x << " " << points.at(i).y << " " << points.at(i).z << "\n";
	}
	pointsFile.close();
}

void Test::write2fPointsInFile(vector<vector<Point2d>> points, string fileName)
{
	fstream projFile(fileName, fstream::trunc | fstream::out);
	projFile.trunc;

	for (int i = 0; i < points.size(); i++) { 
		projFile << "Frame: " << i << "\n";
		for (int j = 0; j < points.at(0).size(); j++) {
			projFile << points.at(i).at(j).x << " " << points.at(i).at(j).y << "\n";
		}
	}
	projFile.close();
}

vector<Point3d> Test::generatePoints(int numberOfPoints)
{
	random_device rd;
    mt19937 gen(rd());
	uniform_int_distribution<> xy(300, 800);
	uniform_int_distribution<> zz(600, 1500);	
	vector<Point3d> points;
	for (int i = 0; i < numberOfPoints; i++) {
		double x = xy(gen);
		double y = xy(gen);
		double z = zz(gen);
		points.push_back(Point3d(x, y, z));
	}
	return points;
}

vector<vector<Point2d>> Test::createProjections(vector<Point3d> trajectory, vector<Point3d> initPoints)
{
	cv::RNG rng;
	vector<vector<Point2d>> projs;
	for (int i = 0; i < trajectory.size(); i++) {
		vector<Point2d> proj;
		for (int j = 0; j < initPoints.size(); j++) {
			Point3d traj = trajectory.at(i);
			Point3d point = initPoints.at(j);
			proj.push_back(Point2d((point.x - traj.x) / point.z + 100.0 //+ rng.gaussian(1)
				, (point.y - traj.y) / point.z + 100.0/* + rng.gaussian(1)*/));
		}
		projs.push_back(proj);
	}
	return projs;
}

vector<vector<Point2d>> Test::createProjections(vector<Point3d> initPoints, vector<Point3d> t
			, vector<Point3d> R, Mat K)
{
	cv::RNG rng;
	vector<vector<Point2d>> projs;
	
	for (int i = 0; i < t.size(); i++) {
		vector<Point2d> proj;
		Mat Rot;
		cv::Rodrigues(Mat(R.at(i)), Rot);
		Mat T = Mat(t.at(i));
		for (int j = 0; j < initPoints.size(); j++) {
			Mat X = Mat(initPoints.at(j));
			Mat xproj = K * (Rot * X + T);
			vector<double> xpr = xproj.col(0);
			if ((xpr.at(2) < 1.0) || (xpr.at(1) < 0.0) || (xpr.at(0) < 0.0) || (xpr.at(0) / xpr.at(2) > 640.0) || (xpr.at(1) / xpr.at(2) > 480.0))
				proj.push_back(Point2d(0, 0));
			else
				proj.push_back(Point2d(xpr.at(0) / xpr.at(2) + rng.gaussian(0), xpr.at(1) / xpr.at(2) + rng.gaussian(0)));
		}
		projs.push_back(proj);
	}
	return projs;
}

vector<Point3d> Test::readTrajectory(string fileName)
{
	fstream trajFile;
	trajFile.open(fileName);

	int numberOfFrames = 0;
	trajFile >> numberOfFrames;
	double tx = 0.0;
	double ty = 0.0;
	double tz = 0.0;
	vector<Point3d> trajectory;
	for (int j = 0; j < numberOfFrames; j++) {
		trajFile >> tx >> ty >> tz;
		trajectory.push_back(Point3d(tx, ty, tz));
	}
	return trajectory;
}

void Test::readTandR(string fileName, vector<Point3d> &t, vector<Point3d> &R)
{
	fstream trajFile;
	trajFile.open(fileName);
	
	int numberOfFrames = 0;
	trajFile >> numberOfFrames;

	double tx = 0.0;
	double ty = 0.0;
	double tz = 0.0;
	double rx = 0.0;
	double ry = 0.0;
	double rz = 0.0;
	for (int j = 0; j < numberOfFrames; j++) {
		trajFile >> tx >> ty >> tz >> rx >> ry >> rz;
		t.push_back(Point3d(tx, ty, tz));
		R.push_back(Point3d(rx, ry, rz));
	}
}

void Test::findRecTrajectory(string fileName)
{
	vector<Point3d> R;
	vector<Point3d> t;
	Test::readTandR(fileName, t, R);
	vector<Point3d> trajectory = Test::readTrajectory(fileName);
	vector<Point3d> initPoints = Test::generatePoints(300);
	Test::write3fPointsInFile(initPoints, "initPointsText.txt");
	Mat K = (Mat_<double>(3, 3) << 320, 0, 320, 0, 320, 240, 0, 0, 1);
	vector<vector<Point2d>> projs = Test::createProjections(initPoints, t, R, K);
	Test::write2fPointsInFile(projs, "projPointsText.txt"); 

	/*Processor newProc;
	vector<Point3d> recTrajectory = newProc.recTrajectory(projs, K);
	Test::write3fPointsInFile(recTrajectory, "trajText.txt");
	vector<Point3d> recPoints = newProc.getTrack().recPoints();
	Test::write3fPointsInFile(recPoints, "recPointsText.txt");

	vector<vector<Point2d>> prrec;
	prrec.push_back(newProc.mProjRecPoints);
	Test::write2fPointsInFile(prrec, "prRecPoints.txt");*/
}

void Test::writeTrajectory(vector<Mat> R, vector<Mat> t, string fileName)
{
	fstream trajFile(fileName, fstream::trunc | fstream::out);
	trajFile.trunc;

	for (int i = 0; i < R.size(); i++) {
		vector<double> r;
		vector<double> tt = t.at(i);
		cv::Rodrigues(R.at(i), r, noArray());
		trajFile << r.at(0) << " " << r.at(1) << " " << r.at(2) << " " << tt.at(0) << " " << tt.at(1) << " " << tt.at(2) << "\n";
	}

	trajFile.close();
}

void Test::findRecTrajectoryReal(string fileName)
{
	vector<vector<Point2d>> frames;
	PTAM::RealData::initFrames(frames, fileName);
	Mat K = (Mat_<double>(3, 3) << 313.1852, 0, 217.9243, 0, 312.6343, 127.7195, 0, 0, 1);
	Processor newProc;
	newProc.recTrajectory(frames, K);

	vector<vector<Point2d>> error = newProc.projError();
	error.erase(error.begin());
	Test::write2fPointsInFile(error, "error.txt");

	vector<Mat> R;
	vector<Mat> t;
	newProc.getTrajectory(R, t);
	Test::writeTrajectory(R, t, "recTraj.txt");
}