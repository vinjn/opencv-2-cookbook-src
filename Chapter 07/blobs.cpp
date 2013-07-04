/*------------------------------------------------------------------------------------------*\
   This file contains material supporting chapter 7 of the cookbook:  
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
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

int main()
{
	// Read input binary image
	cv::Mat image= cv::imread("../binaryGroup.bmp",0);
	if (!image.data)
		return 0; 

	cv::namedWindow("Binary Image");
	cv::imshow("Binary Image",image);

	// Get the contours of the connected components
	std::vector<std::vector<cv::Point>> contours;
	cv::findContours(image, 
		contours, // a vector of contours 
		CV_RETR_EXTERNAL, // retrieve the external contours
		CV_CHAIN_APPROX_NONE); // retrieve all pixels of each contours

	// Print contours' length
	std::cout << "Contours: " << contours.size() << std::endl;
	std::vector<std::vector<cv::Point>>::const_iterator itContours= contours.begin();
	for ( ; itContours!=contours.end(); ++itContours) {

		std::cout << "Size: " << itContours->size() << std::endl;
	}

	// draw black contours on white image
	cv::Mat result(image.size(),CV_8U,cv::Scalar(255));
	cv::drawContours(result,contours,
		-1, // draw all contours
		cv::Scalar(0), // in black
		2); // with a thickness of 2

	cv::namedWindow("Contours");
	cv::imshow("Contours",result);

	// Eliminate too short or too long contours
	int cmin= 100;  // minimum contour length
	int cmax= 1000; // maximum contour length
	std::vector<std::vector<cv::Point>>::const_iterator itc= contours.begin();
	while (itc!=contours.end()) {

		if (itc->size() < cmin || itc->size() > cmax)
			itc= contours.erase(itc);
		else 
			++itc;
	}

	// draw contours on the original image
	cv::Mat original= cv::imread("../group.jpg");
	cv::drawContours(original,contours,
		-1, // draw all contours
		cv::Scalar(255,255,255), // in white
		2); // with a thickness of 2

	cv::namedWindow("Contours on Animals");
	cv::imshow("Contours on Animals",original);

	// Let's now draw black contours on white image
	result.setTo(cv::Scalar(255));
	cv::drawContours(result,contours,
		-1, // draw all contours
		cv::Scalar(0), // in black
		1); // with a thickness of 1
	image= cv::imread("../binaryGroup.bmp",0);

	// testing the bounding box 
	cv::Rect r0= cv::boundingRect(cv::Mat(contours[0]));
	cv::rectangle(result,r0,cv::Scalar(0),2);

	// testing the enclosing circle 
	float radius;
	cv::Point2f center;
	cv::minEnclosingCircle(cv::Mat(contours[1]),center,radius);
	cv::circle(result,cv::Point(center),static_cast<int>(radius),cv::Scalar(0),2);

//	cv::RotatedRect rrect= cv::fitEllipse(cv::Mat(contours[1]));
//	cv::ellipse(result,rrect,cv::Scalar(0),2);

	// testing the approximate polygon
	std::vector<cv::Point> poly;
	cv::approxPolyDP(cv::Mat(contours[2]),poly,5,true);

	std::cout << "Polygon size: " << poly.size() << std::endl;

	// Iterate over each segment and draw it
	std::vector<cv::Point>::const_iterator itp= poly.begin();
	while (itp!=(poly.end()-1)) {
		cv::line(result,*itp,*(itp+1),cv::Scalar(0),2);
		++itp;
	}
	// last point linked to first point
	cv::line(result,*(poly.begin()),*(poly.end()-1),cv::Scalar(20),2);

	// testing the convex hull
	std::vector<cv::Point> hull;
	cv::convexHull(cv::Mat(contours[3]),hull);

	// Iterate over each segment and draw it
	std::vector<cv::Point>::const_iterator it= hull.begin();
	while (it!=(hull.end()-1)) {
		cv::line(result,*it,*(it+1),cv::Scalar(0),2);
		++it;
	}
	// last point linked to first point
	cv::line(result,*(hull.begin()),*(hull.end()-1),cv::Scalar(20),2);

	// testing the moments

	// iterate over all contours
	itc= contours.begin();
	while (itc!=contours.end()) {

		// compute all moments
		cv::Moments mom= cv::moments(cv::Mat(*itc++));

		// draw mass center
		cv::circle(result,
			// position of mass center converted to integer
			cv::Point(mom.m10/mom.m00,mom.m01/mom.m00),
			2,cv::Scalar(0),2); // draw black dot
	}

	cv::namedWindow("Some Shape descriptors");
	cv::imshow("Some Shape descriptors",result);

	// New call to findContours but with CV_RETR_LIST flag
	image= cv::imread("../binaryGroup.bmp",0);

	// Get the contours of the connected components
	cv::findContours(image, 
		contours, // a vector of contours 
		CV_RETR_LIST, // retrieve the external and internal contours
		CV_CHAIN_APPROX_NONE); // retrieve all pixels of each contours

	// draw black contours on white image
	result.setTo(cv::Scalar(255));
	cv::drawContours(result,contours,
		-1, // draw all contours
		cv::Scalar(0), // in black
		2); // with a thickness of 2
	cv::namedWindow("All Contours");
	cv::imshow("All Contours",result);

	cv::waitKey();
	return 0;
}