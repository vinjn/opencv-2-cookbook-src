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

#include "linefinder.h"
#include "edgedetector.h"

#define PI 3.1415926

int main()
{
	// Read input image
	cv::Mat image= cv::imread("../road.jpg",0);
	if (!image.data)
		return 0; 

    // Display the image
	cv::namedWindow("Original Image");
	cv::imshow("Original Image",image);

	// Compute Sobel
	EdgeDetector ed;
	ed.computeSobel(image);

    // Display the Sobel orientation
	cv::namedWindow("Sobel (orientation)");
	cv::imshow("Sobel (orientation)",ed.getSobelOrientationImage());
	cv::imwrite("ori.bmp",ed.getSobelOrientationImage());

    // Display the Sobel low threshold
	cv::namedWindow("Sobel (low threshold)");
	cv::imshow("Sobel (low threshold)",ed.getBinaryMap(125));

    // Display the Sobel high threshold
	cv::namedWindow("Sobel (high threshold)");
	cv::imshow("Sobel (high threshold)",ed.getBinaryMap(350));

	// Apply Canny algorithm
	cv::Mat contours;
	cv::Canny(image,contours,125,350);
	cv::Mat contoursInv;
	cv::threshold(contours,contoursInv,128,255,cv::THRESH_BINARY_INV);

    // Display the image of contours
	cv::namedWindow("Canny Contours");
	cv::imshow("Canny Contours",contoursInv);

	// Create a test image
	cv::Mat test(200,200,CV_8U,cv::Scalar(0));
	cv::line(test,cv::Point(100,0),cv::Point(200,200),cv::Scalar(255));
	cv::line(test,cv::Point(0,50),cv::Point(200,200),cv::Scalar(255));
	cv::line(test,cv::Point(0,200),cv::Point(200,0),cv::Scalar(255));
	cv::line(test,cv::Point(200,0),cv::Point(0,200),cv::Scalar(255));
	cv::line(test,cv::Point(100,0),cv::Point(100,200),cv::Scalar(255));
	cv::line(test,cv::Point(0,100),cv::Point(200,100),cv::Scalar(255));

    // Display the test image
	cv::namedWindow("Test Image");
	cv::imshow("Test Image",test);
	cv::imwrite("test.bmp",test);

	// Hough tranform for line detection
	std::vector<cv::Vec2f> lines;
	cv::HoughLines(contours,lines,1,PI/180,60);

	// Draw the lines
	cv::Mat result(contours.rows,contours.cols,CV_8U,cv::Scalar(255));
	image.copyTo(result);

	std::cout << "Lines detected: " << lines.size() << std::endl;

	std::vector<cv::Vec2f>::const_iterator it= lines.begin();
	while (it!=lines.end()) {

		float rho= (*it)[0];   // first element is distance rho
		float theta= (*it)[1]; // second element is angle theta
		
		if (theta < PI/4. || theta > 3.*PI/4.) { // ~vertical line
		
			// point of intersection of the line with first row
			cv::Point pt1(rho/cos(theta),0);        
			// point of intersection of the line with last row
			cv::Point pt2((rho-result.rows*sin(theta))/cos(theta),result.rows);
			// draw a white line
			cv::line( result, pt1, pt2, cv::Scalar(255), 1); 

		} else { // ~horizontal line

			// point of intersection of the line with first column
			cv::Point pt1(0,rho/sin(theta));        
			// point of intersection of the line with last column
			cv::Point pt2(result.cols,(rho-result.cols*cos(theta))/sin(theta));
			// draw a white line
			cv::line( result, pt1, pt2, cv::Scalar(255), 1); 
		}

		std::cout << "line: (" << rho << "," << theta << ")\n"; 

		++it;
	}

    // Display the detected line image
	cv::namedWindow("Detected Lines with Hough");
	cv::imshow("Detected Lines with Hough",result);

	// Create LineFinder instance
	LineFinder ld;

	// Set probabilistic Hough parameters
	ld.setLineLengthAndGap(100,20);
	ld.setMinVote(80);

	// Detect lines
	std::vector<cv::Vec4i> li= ld.findLines(contours);
	ld.drawDetectedLines(image);
	cv::namedWindow("Detected Lines with HoughP");
	cv::imshow("Detected Lines with HoughP",image);

	std::vector<cv::Vec4i>::const_iterator it2= li.begin();
	while (it2!=li.end()) {

		std::cout << "(" << (*it2)[0] << ","<< (*it2)[1]<< ")-(" 
			     << (*it2)[2]<< "," << (*it2)[3] << ")" <<std::endl;

		++it2;
	}

	// Display one line
	image= cv::imread("../road.jpg",0);
	int n=0;
	cv::line(image, cv::Point(li[n][0],li[n][1]),cv::Point(li[n][2],li[n][3]),cv::Scalar(255),5);
	cv::namedWindow("One line of the Image");
	cv::imshow("One line of the Image",image);

	// Extract the contour pixels of the first detected line
	cv::Mat oneline(image.size(),CV_8U,cv::Scalar(0));
	cv::line(oneline, cv::Point(li[n][0],li[n][1]),cv::Point(li[n][2],li[n][3]),cv::Scalar(255),5);
	cv::bitwise_and(contours,oneline,oneline);
	cv::Mat onelineInv;
	cv::threshold(oneline,onelineInv,128,255,cv::THRESH_BINARY_INV);
	cv::namedWindow("One line");
	cv::imshow("One line",onelineInv);

	std::vector<cv::Point> points;

	// Iterate over the pixels to obtain all point positions
	for( int y = 0; y < oneline.rows; y++ ) {
    
		uchar* rowPtr = oneline.ptr<uchar>(y);
    
		for( int x = 0; x < oneline.cols; x++ ) {

		    // if on a contour
			if (rowPtr[x]) {

				points.push_back(cv::Point(x,y));
			}
		}
    }
	
	// find the best fitting line
	cv::Vec4f line;
	cv::fitLine(cv::Mat(points),line,CV_DIST_L2,0,0.01,0.01);

	std::cout << "line: (" << line[0] << "," << line[1] << ")(" << line[2] << "," << line[3] << ")\n"; 

	int x0= line[2];
	int y0= line[3];
	int x1= x0-200*line[0];
	int y1= y0-200*line[1];
	image= cv::imread("../road.jpg",0);
	cv::line(image,cv::Point(x0,y0),cv::Point(x1,y1),cv::Scalar(0),3);
	cv::namedWindow("Estimated line");
	cv::imshow("Estimated line",image);

	// eliminate inconsistent lines
	ld.removeLinesOfInconsistentOrientations(ed.getOrientation(),0.4,0.1);

   // Display the detected line image
	image= cv::imread("../road.jpg",0);
	ld.drawDetectedLines(image);
	cv::namedWindow("Detected Lines (2)");
	cv::imshow("Detected Lines (2)",image);

	// Create a Hough accumulator
	cv::Mat acc(200,180,CV_8U,cv::Scalar(0));

	// Choose a point
	int x=50, y=30;

	// loop over all angles
	for (int i=0; i<180; i++) {

		double theta= i*PI/180.;

		// find corresponding rho value 
		double rho= x*cos(theta)+y*sin(theta);
		int j= static_cast<int>(rho+100.5);

		std::cout << i << "," << j << std::endl;

		// increment accumulator
		acc.at<uchar>(j,i)++;
	}

	cv::imwrite("hough1.bmp",acc*100);

	// Choose a second point
	x=30, y=10;

	// loop over all angles
	for (int i=0; i<180; i++) {

		double theta= i*PI/180.;
		double rho= x*cos(theta)+y*sin(theta);
		int j= static_cast<int>(rho+100.5);

		acc.at<uchar>(j,i)++;
	}

	cv::namedWindow("Hough Accumulator");
	cv::imshow("Hough Accumulator",acc*100);
	cv::imwrite("hough2.bmp",acc*100);

	// Detect circles
	image= cv::imread("../chariot.jpg",0);
	cv::GaussianBlur(image,image,cv::Size(5,5),1.5);
	std::vector<cv::Vec3f> circles;
	cv::HoughCircles(image, circles, CV_HOUGH_GRADIENT, 
		2,   // accumulator resolution (size of the image / 2) 
		50,  // minimum distance between two circles
		200, // Canny high threshold 
		100, // minimum number of votes 
		25, 100); // min and max radius

	std::cout << "Circles: " << circles.size() << std::endl;
	
	// Draw the circles
	image= cv::imread("../chariot.jpg",0);
	std::vector<cv::Vec3f>::const_iterator itc= circles.begin();
	
	while (itc!=circles.end()) {
		
	  cv::circle(image, 
		  cv::Point((*itc)[0], (*itc)[1]), // circle centre
		  (*itc)[2], // circle radius
		  cv::Scalar(255), // color 
		  2); // thickness
		
	  ++itc;	
	}

	cv::namedWindow("Detected Circles");
	cv::imshow("Detected Circles",image);

	cv::waitKey();
	return 0;
}