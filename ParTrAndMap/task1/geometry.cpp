#include "stdafx.h"

#include "cv.h"
#include "cxcore.h"
#include "highgui.h"

#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include "geometry.h"

using namespace cv;
using namespace std;
using namespace PTAM;

bool Geometry::rightSolution(vector<double> x1, vector<double> x2, vector<double> t)
{
	if (x2.at(2) < 0.0) {
		return false;
	}
	return ((x2.at(0) - x2.at(1)) * t.at(2) - (t.at(0) - t.at(1)) / (x1.at(0) - x1.at(1) - x2.at(0) + x2.at(1))) > 0.0;
}

bool Geometry::chooseE(Mat u, Mat W, Mat vt, int direct, vector<Point2f> points1, vector<Point2f> points2, Mat K, Mat &R, Mat &t)
{
	R = u * W * vt;
	Mat u3 = (Mat_<double>(1,3) << 0, 0, 1);
	t = direct * u * u3.t();
	
	Mat x = (Mat_<double>(3,1) << points1.at(0).x, points1.at(0).y, 1);
	Mat xx = (Mat_<double>(3,1) << points2.at(0).x, points2.at(0).x, 1);
	
	Mat x1 = K.inv() * x;
	Mat x2 = R.inv() * (K.inv() * xx - t);

	return rightSolution(x1, x2, t);
}

Mat Geometry::findE(vector<Point2f> points1, vector<Point2f> points2, Mat K)
{
	Mat F = findFundamentalMat(Mat(points1), Mat(points2), CV_FM_RANSAC, 3.0, 0.99, noArray());

	Mat KT(3, 3, DataType<double>::type);
	KT = K.t();
	Mat E = KT * F * K;

	vector<double> firstEl = E.col(0).row(0);
	if (firstEl.at(0) <= 0.0) 
		E = E * (-1.0);

	cout << E;
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

Point3f Geometry::triangulateOnePoint(Mat P1, Mat P2, Point2f proj1, Point2f proj2)
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

	return Point3f(vector<double>(x.row(0).col(0)).at(0), vector<double>(x.row(1).col(0)).at(0), vector<double>(x.row(2).col(0)).at(0));
}

vector<Point3f> Geometry::triangulatePoints(vector<Point2f> points1, vector<Point2f> points2, Mat K, Mat R, Mat t)
{
	Mat P1 = composeProjMatrix(K, Mat::eye(3, 3,  CV_8U), Mat::zeros(3, 1,  CV_8U));
	Mat P2 = composeProjMatrix(K, R, t);

	vector<Point3f> initPoints(0);
	for (int i = 0; i < points1.size(); i++) {
		initPoints.push_back(Geometry::triangulateOnePoint(P1, P2, points1.at(i), points2.at(i)));
	}

	return initPoints;
}

void Geometry::findRt(vector<Point2f> points1, vector<Point2f> points2, Mat K, Mat &R, Mat &t)
{
	Mat E = Geometry::findE(points1, points2, K);

	Mat w, u, vt;
	SVDecomp(E, w, u, vt);

	Mat W = (Mat_<double>(3,3) << 0, -1, 0, 1, 0, 0, 0, 0, 1);
	
	//4 варианта геометрического положения
	if (!Geometry::chooseE(u, W, vt, 1, points1, points2, K, R, t))
		if (!Geometry::chooseE(u, W, vt, -1, points1, points2, K, R, t))
			if (!Geometry::chooseE(u, W.t(), vt, 1, points1, points2, K, R, t))
				Geometry::chooseE(u, W.t(), vt, -1, points1, points2, K, R, t);

	cout << R;
 }