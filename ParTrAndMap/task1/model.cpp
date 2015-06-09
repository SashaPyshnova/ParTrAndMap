#include "stdafx.h"

#include <opencv2/opencv.hpp> 
#include "cxcore.h"
#include "highgui.h"

#include <stdlib.h>
#include <stdio.h>
#include <random>
#include <math.h>
#include "geometry.h"
#include "model.h"
#include <fstream>
#include <iostream>

using namespace cv;
using namespace std;
using namespace PTAM;

void Model::SevenPointsMethod()
{
	/*7ми точечный метод для поиска фундаментальной матрицы 
	вторая камера смещена по оси х на 100*/

	int pointCount = 7;
	vector<Point2d> points1(pointCount);
	vector<Point2d> points2(pointCount);

	points1[0] = Point2d(50, 50);
	points2[0] = Point2d(49, 50);
	points1[1] = Point2d(50, 50);
	points2[1] = Point2d(49.5, 50);
	points1[2] = Point2d(51, 51);
	points2[2] = Point2d(41, 51);
	points1[3] = Point2d(51, 51);
	points2[3] = Point2d(46, 51);
	points1[4] = Point2d(50, 0);
	points2[4] = Point2d(0, 0);
	points1[5] = Point2d(40, 20);
	points2[5] = Point2d(20, 20);
	points1[6] = Point2d(60, 10);
	points2[6] = Point2d(10, 10);

	Mat fundamentalMatrix = findFundamentalMat(Mat(points1), Mat(points2), CV_FM_7POINT);
	cout << fundamentalMatrix;
}

void Model::writeProjPointsInFile(vector<Point2d> points1, vector<Point2d> points2)
{
	fstream projFile("pointsProj.txt", fstream::trunc | fstream::out);
	projFile.trunc;
	projFile << points1.size() << "\n";
	for (int i = 0; i < points1.size(); i++) {
		projFile << points1.at(i).x << " " << points1.at(i).y << " " << points2.at(i).x << " " << points2.at(i).y << "\n";
	}
	projFile.close();
}

void Model::writeInitPointsInFile(vector<Point3d> initPoints, string fileName)
{
	fstream initPointsFile(fileName, fstream::trunc | fstream::out);
	initPointsFile.trunc;
	initPointsFile << initPoints.size() << "\n";
	for (int i = 0; i < initPoints.size(); i++) {
		initPointsFile << initPoints.at(i).x << " " << initPoints.at(i).y << " " << initPoints.at(i).z << "\n";
	}
	initPointsFile.close();
}

void Model::readFromFile(vector<vector<Point2d>> &projPoints)
{
	fstream pointsProjFile;
	pointsProjFile.open("SQProj.txt");

	int numberOfPoints = 0;
	pointsProjFile >> numberOfPoints;
	int numberOfFrames = 0;
	pointsProjFile >> numberOfFrames;
	double x = 0.0;
	double y = 0.0;
	for (int j = 0; j < numberOfFrames; j++) {
		vector<Point2d> frame;
		for (int i = 0; i < numberOfPoints; i++) {
			pointsProjFile >> x >> y;
			frame.push_back(Point2d(x, y));
		}
		projPoints.push_back(frame);
	}
}

void Model::generatePoints(vector<Point2d> &points1, vector<Point2d> &points2, vector<Point3d> &initPoints)
{
	random_device rd;
    mt19937 gen(rd());
	uniform_int_distribution<> dst(1,100);
	int index = 0;
	int cx = 50;
	int cy = 50;
	for (int u = 50; u < 101; u = u + 5) {
		for (int v = 50; v < 101; v = v + 5) {
			double z = dst(gen);
			double x = z * (1.0 * u - 50.0);
			if ((x > -50.0 * z + 100.0) && (x <= 50.0 * z)) {
				points1.push_back(Point2d(x / z + 50.0, v));
				points2.push_back(Point2d((x - 100.0) / z + 50.0, v)); 
				initPoints.push_back(Point3d(x, (v * 1.0 - 50.0) * z, z));
			}
		}
		/*for (int v = 50; v < 101; v = v + 5) {
			int z = dst(gen);
			int x = z * (u - 50);
			if ((x > -50 * z + 100) && (x <= 50 * z)) {
				points1.push_back(Point2d(x / z + 50, v));
				points2.push_back(Point2d((x - 100) / z + 50, v));
				initPoints.push_back(Point3d(x, (v - 50) * z, z));
			}
		}*/
	}

	Model::writeInitPointsInFile(initPoints, "initPoints.txt");
	Model::writeProjPointsInFile(points1, points2);
}

void Model::findRightRAndt()
{
	vector<vector<Point2d>> projPoints(0);
	vector<Point3d> initPoints(0);
	//Model::generatePoints(points1, points2, initPoints);
	Model::readFromFile(projPoints);

	Mat const K = (Mat_<double>(3,3) << 1, 0, 50, 0, 1, 50, 0, 0, 1);
	Mat R = Mat_<double>(3, 3);
	Mat t = Mat_<double>(3, 1);
	Geometry::findRt(projPoints.at(0), projPoints.at(1), K, R, t);
	cout << R << "\n" << t << "\n";

	vector<Point3d> initPointsRe = Geometry::triangulatePoints(projPoints.at(0), projPoints.at(1), K, R, t);
	//Model::writeInitPointsInFile(initPointsRe, "SQRe.txt");
	Mat R3;
	Mat t3;
	Geometry::findRtUsingInitPoints(initPointsRe, projPoints.at(2), R3, t3, K);
	cout << R3 << "\n" << t3 << "\n"; 
}

