#include "stdafx.h"

#include <opencv2/opencv.hpp> 
#include "cxcore.h"
#include "highgui.h"

#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include "geometry.h"

using namespace cv;
using namespace std;
using namespace PTAM;

bool Geometry::rightSolution(vector<double> &x1, vector<double> &x2, vector<double> &t)
{
	double r = (x2.at(0) - x2.at(1)) * t.at(2) - (t.at(0) - t.at(1)) / (x1.at(0) - x1.at(1) - x2.at(0) + x2.at(1));
	return r > 0.0;
}

bool Geometry::chooseE(Mat u, Mat W, Mat vt, int direct, vector<Point2d> &points1, vector<Point2d> &points2
					   , Mat K, Mat &R, Mat &t, int index)
{
	R = u * W * vt;
	Mat u3 = (Mat_<double>(1,3) << 0, 0, 1);
	t = direct * u * u3.t();
	double sign = vector<double>(R.col(0).row(0)).at(0);
	if (sign < 0.0) {
		R = R * (-1.0);
		t = t * (-1.0);
	}

//	cout << "R:\n" << R << "\n:t:\n" << t;
	
	Mat P1 = composeProjMatrix(K, Mat::eye(3, 3,  CV_8U), Mat::zeros(3, 1,  CV_8U));
	Mat P2 = composeProjMatrix(K, R, t);
	Point3d X = Geometry::triangulateOnePoint(P1, P2, points1.at(index), points2.at(index));

	if (X.z < 0.0)
		return false;


	Mat x = (Mat_<double>(3,1) << points1.at(index).x, points1.at(index).y, 1);
	Mat xx = (Mat_<double>(3,1) << points2.at(index).x, points2.at(index).y, 1);
	
	Mat x1 = x;
	Mat x2 = R.inv() * (xx - t);
	
	//cout << "\n" << x1 << "\n" << x2;
	return ((Point3d(x1).y * Point3d(x2).y > 0.0));
	//return rightSolution(x1, x2, t);
}

Mat Geometry::findE(vector<Point2d> &points1, vector<Point2d> &points2, Mat K, Mat &wrongPoints)
{
	vector<Point2d> points1n;
	vector<Point2d> points2n;
	/*for (int i = 0; i < points1.size(); i++) {
		if ((points1.at(i) != Point2d(0, 0)) && (points2.at(i) != Point2d(0, 0))) {
			points1n.push_back(points1.at(i));
			points2n.push_back(points2.at(i));
		}
	}*/

	Mat F = findFundamentalMat(Mat(points1), Mat(points2), CV_FM_RANSAC, 3.0, 0.99, wrongPoints);
	cout << wrongPoints;
//	cout << sum(wrongPoints);
//	cout << F;
	Mat KT(3, 3, DataType<double>::type);
	KT = K.t();
	Mat E = KT * F * K;
//	cout << E;

	return E;
}

Mat Geometry::composeProjMatrix(Mat K, Mat R, Mat t)
{
	vector<double> r;
	vector<double> elem;

	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			elem = R.row(i).col(j);
			r.push_back(elem.at(0));
		}
		elem = t.row(i).col(0);
		r.push_back(elem.at(0));
	}

	Mat P = K * Mat(r).reshape(0, 3);
	return P;
}

Point3d Geometry::triangulateOnePoint(Mat P1, Mat P2, Point2d proj1, Point2d proj2)
{
	Mat a0 = proj1.x * P1.row(2) - P1.row(0);
	Mat a1 = proj1.y * P1.row(2) - P1.row(1);
	Mat a2 = proj2.x * P2.row(2) - P2.row(0);
	Mat a3 = proj2.y * P2.row(2) - P2.row(1);
	
	a0.push_back(a1);
	a0.push_back(a2);
	a0.push_back(a3);

	Mat b = a0.col(3);
	Mat A = a0.colRange(0, 3);

	Mat x = (A.t() * A).inv() * A.t() * (Mat(b) * (-1.0));

	return Point3d(vector<double>(x.row(0).col(0)).at(0), vector<double>(x.row(1).col(0)).at(0), vector<double>(x.row(2).col(0)).at(0));
}

vector<Point3d> Geometry::triangulatePoints(vector<Point2d> &points1, vector<Point2d> &points2, Mat K, Mat &R, Mat &t)
{
	Mat P1 = composeProjMatrix(K, Mat::eye(3, 3,  CV_8U), Mat::zeros(3, 1,  CV_8U));
	Mat P2 = composeProjMatrix(K, R, t);

	vector<Point3d> initPoints(0);
	for (int i = 0; i < points1.size(); i++) {
		Point3d recPoint = Geometry::triangulateOnePoint(P1, P2, points1.at(i), points2.at(i));
		initPoints.push_back(recPoint);
	}

	return initPoints;
}

void Geometry::setParam(cv::Mat u, cv::Mat W, cv::Mat vt, int direct, cv::Mat &R, cv::Mat &t)
{
	R = u * W * vt;
	Mat u3 = (Mat_<double>(1,3) << 0, 0, 1);
	t = direct * u * u3.t();
	double sign = vector<double>(R.col(0).row(0)).at(0);
	if (sign < 0.0) {
		R = R * (-1.0);
		t = t * (-1.0);
	}
}

void Geometry::findRt(vector<Point2d> &points1, vector<Point2d> &points2, Mat K, Mat &R, Mat &t)
{
	Mat wrongPoints;
	Mat E = Geometry::findE(points1, points2, K, wrongPoints);

	Mat w, u, vt;
	SVDecomp(E, w, u, vt);

	Mat W = (Mat_<double>(3,3) << 0, -1, 0, 1, 0, 0, 0, 0, 1);
	
	int Var1 = 0;
	int Var2 = 0;
	int Var3 = 0;
	int Var4 = 0;
	
	std::valarray<int> var(4);

	vector<int> wp = wrongPoints;
	for (int i = 0; i < points1.size(); i++) {
		if (wp[i] == 1) {
			if (Geometry::chooseE(u, W, vt, 1, points1, points2, K, R, t, i)) 
				var[0] += 1;
			else {
				if (Geometry::chooseE(u, W, vt, -1, points1, points2, K, R, t, i))
					var[1] += 1;
				else {
					if (Geometry::chooseE(u, W.t(), vt, 1, points1, points2, K, R, t, i))
						var[2] += 1;
					else
						if (Geometry::chooseE(u, W.t(), vt, -1, points1, points2, K, R, t, i))
							var[3] += 1;
				}
			} 
		}
	}

	int max = var.max();
	if (var[0] == max)
		Geometry::setParam(u, W, vt, 1, R, t);
	else if (var[1] == max)
			Geometry::setParam(u, W, vt, -1, R, t);
		else if (var[2] == max)
			Geometry::setParam(u, W.t(), vt, 1, R, t);
			else Geometry::setParam(u, W.t(), vt, -1, R, t);
	/*//4 варианта геометрического положения
	if (!Geometry::chooseE(u, W, vt, 1, points1, points2, K, R, t)) {
		if (!Geometry::chooseE(u, W, vt, -1, points1, points2, K, R, t)) {
			if (!Geometry::chooseE(u, W.t(), vt, 1, points1, points2, K, R, t)) {
				Geometry::chooseE(u, W.t(), vt, -1, points1, points2, K, R, t);
			}
		}
	}
	*/
	//cout << "R:\n" << R << "\n:t:\n" << t;
 }

void Geometry::findRtUsingInitPoints(vector<Point3d> &initPoints, vector<Point2d> &pointsProj, Mat &R, Mat &t, Mat K)
{
	Mat rvec;
	cv::solvePnP(initPoints, pointsProj, K, noArray(), rvec, t);
	cv::Rodrigues(rvec, R, noArray());
}