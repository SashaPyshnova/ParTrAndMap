// task1.cpp: определяет точку входа для консольного приложения.
//

#include "stdafx.h"

#include "cv.h"
#include "cxcore.h"
#include "highgui.h"

#include <stdlib.h>
#include <stdio.h>
#include <random>

using namespace cv;
using namespace std;

void SevenPointsMethod()
{
	/*7ми точечный метод для поиска фундаментальной матрицы 
	вторая камера смещена по оси х на 100*/

	int pointCount = 7;
	vector<Point2f> points1(pointCount);
	vector<Point2f> points2(pointCount);

	points1[0] = Point2f(50, 50);
	points2[0] = Point2f(49, 50);
	points1[1] = Point2f(50, 50);
	points2[1] = Point2f(49.5, 50);
	points1[2] = Point2f(51, 51);
	points2[2] = Point2f(41, 51);
	points1[3] = Point2f(51, 51);
	points2[3] = Point2f(46, 51);
	points1[4] = Point2f(50, 0);
	points2[4] = Point2f(0, 0);
	points1[5] = Point2f(40, 20);
	points2[5] = Point2f(20, 20);
	points1[6] = Point2f(60, 10);
	points2[6] = Point2f(10, 10);

	Mat fundamentalMatrix = findFundamentalMat(Mat(points1), Mat(points2), CV_FM_7POINT);
	cout <<  fundamentalMatrix;
}

void RANSACMethod()
{
	random_device rd;
    mt19937 gen(rd());
	uniform_int_distribution<> dst(1,100);
	int index = 0;
	int cx = 50;
	int cy = 50;
	vector<Point2f> points1(0);
	vector<Point2f> points2(0);
	for (int u = 0; u < 101; u = u + 10) {
		for (int v = 0; v < 101; v = v + 10) {
			int z = dst(gen);
			int x = z * (u - 50);
			if ((x > -50 * z + 100) && (x <= 50 * z)) {
				points1.push_back(Point(x / z + 50, v));
				points2.push_back(Point((x - 100) / z + 50, v));
			}
		}
	}
	Mat fundamentalMatrix = findFundamentalMat(Mat(points1), Mat(points2), CV_FM_RANSAC, 3.0, 0.99, noArray());
	cout << fundamentalMatrix;
	cout << "";
}

int _tmain(int argc, _TCHAR* argv[])
{
	SevenPointsMethod();
	RANSACMethod();
	return 0;
}

