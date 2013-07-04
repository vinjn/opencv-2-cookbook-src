/*------------------------------------------------------------------------------------------*\
   This file contains material supporting chapter 2 of the cookbook:  
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


#include <vector>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>


int main()
{
	cv::Mat image1;
	cv::Mat image2;

	image1= cv::imread("boldt.jpg");
	image2= cv::imread("rain.jpg");
	if (!image1.data)
		return 0; 
	if (!image2.data)
		return 0; 

	cv::namedWindow("Image 1");
	cv::imshow("Image 1",image1);
	cv::namedWindow("Image 2");
	cv::imshow("Image 2",image2);

	cv::Mat result;
	cv::addWeighted(image1,0.7,image2,0.9,0.,result);

	cv::namedWindow("result");
	cv::imshow("result",result);

	// using overloaded operator
	result= 0.7*image1+0.9*image2;

	cv::namedWindow("result with operators");
	cv::imshow("result with operators",result);

	image2= cv::imread("rain.jpg",0);

	// create vector of 3 images
	std::vector<cv::Mat> planes;
	// split 1 3-channel image into 3 1-channel images
	cv::split(image1,planes);
	// add to blue channel
	planes[0]+= image2;
	// merge the 3 1-channel images into 1 3-channel image
	cv::merge(planes,result);

	cv::namedWindow("Result on blue channel");
	cv::imshow("Result on blue channel",result);

	// read images
	cv::Mat image= cv::imread("boldt.jpg");
	cv::Mat logo= cv::imread("logo.bmp");

	// define image ROI
	cv::Mat imageROI;
	imageROI= image(cv::Rect(385,270,logo.cols,logo.rows));

	// add logo to image 
	cv::addWeighted(imageROI,1.0,logo,0.3,0.,imageROI);

	// show result
	cv::namedWindow("with logo");
	cv::imshow("with logo",image);

	// read images
	image= cv::imread("boldt.jpg");
	logo= cv::imread("logo.bmp");

	// define ROI
	imageROI= image(cv::Rect(385,270,logo.cols,logo.rows));

	// load the mask (must be gray-level)
	cv::Mat mask= cv::imread("logo.bmp",0);

	// copy to ROI with mask
	logo.copyTo(imageROI,mask);

	// show result
	cv::namedWindow("with logo 2");
	cv::imshow("with logo 2",image);

	// read images
	logo= cv::imread("logo.bmp",0);
	image1= cv::imread("boldt.jpg");

	// split 3-channel image into 3 1-channel images
	std::vector<cv::Mat> channels;
	cv::split(image1,channels);

	imageROI= channels.at(1);

	cv::addWeighted(imageROI(cv::Rect(385,270,logo.cols,logo.rows)),1.0,
			        logo,0.5,0.,imageROI(cv::Rect(385,270,logo.cols,logo.rows)));

	cv::merge(channels,image1);

	cv::namedWindow("with logo 3");
	cv::imshow("with logo 3",image1);

	cv::waitKey();

	return 0;
}
