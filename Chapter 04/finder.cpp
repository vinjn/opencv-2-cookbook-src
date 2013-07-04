/*------------------------------------------------------------------------------------------*\
   This file contains material supporting chapter 4 of the cookbook:  
   Computer Vision Programming using the OpenCV Library. 
   by Robert Laganiere, Packt Publishing, 2011.

   This program is free software; permission is hereby granted to use, copy, modify, 
   and distribute this source code, or portions thereof, for any purpose, without fee, 
   subject to the restriction that the copyright notice may not be removed 
   or altered from any source or altered source distribution. 
   The software is released on an as-is basis and without any warranties of any kind. 
   In particular, the software is not guaranteed to be fault-tolerant or free from failure. 
   The author disclaims all warranties with regard to this software, any use, 
   and any consequent failure, is purely the responsibility of the user.
 
   Copyright (C) 2010-2011 Robert Laganiere, www.laganiere.name
\*------------------------------------------------------------------------------------------*/

#include <iostream>
#include <vector>
using namespace std;

#include <opencv2\core\core.hpp>
#include <opencv2\highgui\highgui.hpp>
#include <opencv2\imgproc\imgproc.hpp>
#include <opencv2\video\tracking.hpp>

#include "objectFinder.h"
#include "colorhistogram.h"

int main()
{
	// Read reference image
	cv::Mat image= cv::imread("../baboon1.jpg");
	if (!image.data)
		return 0; 

	// Define ROI
	cv::Mat imageROI= image(cv::Rect(110,260,35,40));
	cv::rectangle(image, cv::Rect(110,260,35,40),cv::Scalar(0,0,255));

	// Display image
	cv::namedWindow("Image");
	cv::imshow("Image",image);

	// Get the Hue histogram
	int minSat=65;
	ColorHistogram hc;
	cv::MatND colorhist= hc.getHueHistogram(imageROI,minSat);

	ObjectFinder finder;
	finder.setHistogram(colorhist);
	finder.setThreshold(0.2f);

	// Convert to HSV space
	cv::Mat hsv;
	cv::cvtColor(image, hsv, CV_BGR2HSV);

	// Split the image
	vector<cv::Mat> v;
	cv::split(hsv,v);

	// Eliminate pixels with low saturation
	cv::threshold(v[1],v[1],minSat,255,cv::THRESH_BINARY);
	cv::namedWindow("Saturation");
	cv::imshow("Saturation",v[1]);

	// Get back-projection of hue histogram
	int ch[1]={0};
	cv::Mat result= finder.find(hsv,0.0f,180.0f,ch,1);

	cv::namedWindow("Result Hue");
	cv::imshow("Result Hue",result);

	cv::bitwise_and(result,v[1],result);
	cv::namedWindow("Result Hue and");
	cv::imshow("Result Hue and",result);

	// Second image
	image= cv::imread("../baboon3.jpg");

	// Display image
	cv::namedWindow("Image 2");
	cv::imshow("Image 2",image);

	// Convert to HSV space
	cv::cvtColor(image, hsv, CV_BGR2HSV);

	// Split the image
	cv::split(hsv,v);

	// Eliminate pixels with low saturation
	cv::threshold(v[1],v[1],minSat,255,cv::THRESH_BINARY);
	cv::namedWindow("Saturation");
	cv::imshow("Saturation",v[1]);

	// Get back-projection of hue histogram
	result= finder.find(hsv,0.0f,180.0f,ch,1);

	cv::namedWindow("Result Hue");
	cv::imshow("Result Hue",result);

	// Eliminate low stauration pixels
	cv::bitwise_and(result,v[1],result);
	cv::namedWindow("Result Hue and");
	cv::imshow("Result Hue and",result);

	// Get back-projection of hue histogram
	finder.setThreshold(-1.0f);
	result= finder.find(hsv,0.0f,180.0f,ch,1);
	cv::bitwise_and(result,v[1],result);
	cv::namedWindow("Result Hue and raw");
	cv::imshow("Result Hue and raw",result);

	cv::Rect rect(110,260,35,40);
	cv::rectangle(image, rect, cv::Scalar(0,0,255));

	cv::TermCriteria criteria(cv::TermCriteria::MAX_ITER,10,0.01);
	cout << "meanshift= " << cv::meanShift(result,rect,criteria) << endl;

	cv::rectangle(image, rect, cv::Scalar(0,255,0));

	// Display image
	cv::namedWindow("Image 2 result");
	cv::imshow("Image 2 result",image);

	cv::waitKey();
	return 0;
}