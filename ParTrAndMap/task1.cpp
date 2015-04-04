#include "stdafx.h"

#include "cv.h"
#include "cxcore.h"
#include "highgui.h"

#include <stdlib.h>
#include <stdio.h>

IplImage* image = 0;
IplImage* resizeImage[4];

int _tmain(int argc, _TCHAR* argv[])
{
	char* filename = "image.png";
	image = cvLoadImage(filename,1);

	resizeImage[0] = cvCreateImage(cvSize(image -> width * 2, image -> height * 2), image -> depth, image -> nChannels);
    cvResize(image, resizeImage[0], 0); //nn 
	resizeImage[1] = cvCreateImage(cvSize(image -> width * 2, image -> height * 2), image -> depth, image -> nChannels);
    cvResize(image, resizeImage[1], 3); //bb
	resizeImage[2] = cvCreateImage(cvSize(image -> width / 2, image -> height / 2), image -> depth, image -> nChannels);
    cvResize(image, resizeImage[2], 0);
	resizeImage[3] = cvCreateImage(cvSize(image -> width / 2, image -> height / 2), image -> depth, image -> nChannels);
    cvResize(image, resizeImage[3], 3);

	/*cvSaveImage("upnn.jpg", resizeImage[0], 0);
	cvSaveImage("upbb.jpg", resizeImage[1], 0);
	cvSaveImage("downnn.jpg", resizeImage[2], 0);
	cvSaveImage("downbb.jpg", resizeImage[3], 0);*/

	return 0;
}