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
using namespace std;

#include "cv.h"
#include "highgui.h"

#include "histogram.h"
#include "objectFinder.h"
#include "colorhistogram.h"

int main()
{
	// Read input image
	cv::Mat image= cv::imread("../waves.jpg",0);
	if (!image.data)
		return 0; 

	// define image ROI
	cv::Mat imageROI;
	imageROI= image(cv::Rect(360,55,40,50)); // Cloud region

	// Display reference patch
	cv::namedWindow("Reference");
	cv::imshow("Reference",imageROI);

	// Find histogram of reference
	Histogram1D h;
	cv::MatND hist= h.getHistogram(imageROI);
	cv::namedWindow("Reference Hist");
	cv::imshow("Reference Hist",h.getHistogramImage(imageROI));

	// Create the objectfinder
	ContentFinder finder;
	finder.setHistogram(hist);

	finder.setThreshold(-1.0f);

	// Get back-projection
	cv::Mat result1;
	result1= finder.find(image);

	// Create negative image and display result
	cv::Mat tmp;
	result1.convertTo(tmp,CV_8U,-1.0,255.0);
	cv::namedWindow("Backprojection result");
	cv::imshow("Backprojection result",tmp);

	// Get binary back-projection
	finder.setThreshold(0.12f);
	result1= finder.find(image);

	// Draw a rectangle around the reference area
	cv::rectangle(image,cv::Rect(360,55,40,50),cv::Scalar(0,0,0));

	// Display image
	cv::namedWindow("Image");
	cv::imshow("Image",image);

	// Display result
	cv::namedWindow("Detection Result");
	cv::imshow("Detection Result",result1);

	// Second test image
	cv::Mat image2= cv::imread("../dog.jpg",0);
	cv::Mat result2;
	result2= finder.find(image2);

	// Display result
	cv::namedWindow("Result (2)");
	cv::imshow("Result (2)",result2);

	// Load color image
	ColorHistogram hc;
	cv::Mat color= cv::imread("../waves.jpg");
	color= hc.colorReduce(color,32);
	cv::namedWindow("Color Image");
	cv::imshow("Color Image",color);

	imageROI= color(cv::Rect(0,0,165,75)); // blue sky area

	// Get 3D color histogram
	cv::MatND shist= hc.getHistogram(imageROI);
	// Histograms with SparseMat does not work with OpenCV2.1
	// cv::SparseMat shist= hc.getSparseHistogram(imageROI);

	finder.setHistogram(shist);
	finder.setThreshold(0.05f);

	// Get back-projection of color histogram
	result1= finder.find(color);

	cv::namedWindow("Color Backproject Result");
	cv::imshow("Color Backproject Result",result1);

	// Second color image
	cv::Mat color2= cv::imread("../dog.jpg");
	color2= hc.colorReduce(color2,32);

	// Get back-projection of color histogram
	result2= finder.find(color2);

	cv::namedWindow("Result color (2)");
	cv::imshow("Result color (2)",result2);

	// Get ab color histogram
	color= cv::imread("../waves.jpg");
	imageROI= color(cv::Rect(0,0,165,75)); // blue sky area
	cv::MatND colorhist= hc.getabHistogram(imageROI);

	finder.setHistogram(colorhist);
	finder.setThreshold(0.05f);

	// Convert to Lab space
	cv::Mat lab;
	cv::cvtColor(color, lab, CV_BGR2Lab);

	// Get back-projection of ab histogram
	int ch[2]={1,2};
	result1= finder.find(lab,-128.0f,127.0f,ch,2);

	cv::namedWindow("Result ab (1)");
	cv::imshow("Result ab (1)",result1);

	// Second color image
	color2= cv::imread("../dog.jpg");

	cv::namedWindow("Color Image (2)");
	cv::imshow("Color Image (2)",color2);

	cv::cvtColor(color2, lab, CV_BGR2Lab);

	result2= finder.find(lab,-128.0f,127.0f,ch,2);

	cv::namedWindow("Result ab (2)");
	cv::imshow("Result ab (2)",result2);

	// Get Hue color histogram
	color= cv::imread("../waves.jpg");
	imageROI= color(cv::Rect(0,0,165,75)); // blue sky area
	colorhist= hc.getHueHistogram(imageROI);

	finder.setHistogram(colorhist);
	finder.setThreshold(0.3f);

	// Convert to HSV space
	cv::Mat hsv;
	cv::cvtColor(color, hsv, CV_BGR2HSV);

	// Get back-projection of hue histogram
	ch[0]=0;
	result1= finder.find(hsv,0.0f,180.0f,ch,1);

	cv::namedWindow("Result Hue (1)");
	cv::imshow("Result Hue (1)",result1);

	// Second color image
	color2= cv::imread("../dog.jpg");

	cv::cvtColor(color2, hsv, CV_BGR2HSV);

	result2= finder.find(hsv,0.0f,180.0f,ch,1);

	cv::namedWindow("Result Hue (2)");
	cv::imshow("Result Hue (2)",result2);

	cv::waitKey();
	return 0;
}

