#include "stdafx.h"

#include "test.h"
#include "processor.h"

#include "cv.h"
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

void Test::write3fPointsInFile(vector<Point3f> points, string fileName)
{
	fstream pointsFile(fileName, fstream::trunc | fstream::out);
	pointsFile.trunc;
	for (int i = 0; i < points.size(); i++) {
		pointsFile << points.at(i).x << " " << points.at(i).y << " " << points.at(i).z << "\n";
	}
	pointsFile.close();
}

void Test::write2fPointsInFile(vector<vector<Point2f>> points, string fileName)
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

vector<Point3f> Test::generatePoints(int numberOfPoints)
{
	random_device rd;
    mt19937 gen(rd());
	uniform_int_distribution<> xy(0, 300);
	uniform_int_distribution<> zz(20, 100);	
	vector<Point3f> points;
	for (int i = 0; i < numberOfPoints; i++) {
		double x = xy(gen);
		double y = xy(gen);
		double z = zz(gen);
		points.push_back(Point3f(x, y, z));
	}
	return points;
}

vector<vector<Point2f>> Test::createProjections(vector<Point3f> trajectory, vector<Point3f> initPoints)
{
	vector<vector<Point2f>> projs;
	for (int i = 0; i < trajectory.size(); i++) {
		vector<Point2f> proj;
		for (int j = 0; j < initPoints.size(); j++) {
			Point3f traj = trajectory.at(i);
			Point3f point = initPoints.at(j);
			proj.push_back(Point2f((point.x - traj.x) / point.z + 50.0, (point.y - traj.y) / point.z + 50.0));
		}
		projs.push_back(proj);
	}
	return projs;
}

vector<Point3f> Test::readTrajectory(string fileName)
{
	fstream trajFile;
	trajFile.open(fileName);

	int numberOfFrames = 0;
	trajFile >> numberOfFrames;
	double tx = 0.0;
	double ty = 0.0;
	double tz = 0.0;
	vector<Point3f> trajectory;
	for (int j = 0; j < numberOfFrames; j++) {
		trajFile >> tx >> ty >> tz;
		trajectory.push_back(Point3f(tx, ty, tz));
	}
	return trajectory;
}

void Test::findRecTrajectory(string fileName)
{
	vector<Point3f> trajectory = Test::readTrajectory(fileName);
	vector<Point3f> initPoints = Test::generatePoints(100);
	Test::write3fPointsInFile(initPoints, "initPointsForTraj.txt");
	vector<vector<Point2f>> projs = Test::createProjections(trajectory, initPoints);
	Test::write2fPointsInFile(projs, "projPointsForTraj.txt"); 

	Processor newProc;
	vector<Point3f> recTrajectory = newProc.recTrajectory(projs);
	Test::write3fPointsInFile(recTrajectory, "recTrajectory.txt");
}