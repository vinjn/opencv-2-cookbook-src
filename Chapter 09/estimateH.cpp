/*------------------------------------------------------------------------------------------*\
   This file contains material supporting chapter 9 of the cookbook:  
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
#include <opencv2/features2d/features2d.hpp>
#include <opencv2/calib3d/calib3d.hpp>
#include "matcher.h"

int main()
{
	// Read input images
	cv::Mat image1= cv::imread("../parliament1.bmp",0);
	cv::Mat image2= cv::imread("../parliament2.bmp",0);
	if (!image1.data || !image2.data)
		return 0; 

    // Display the images
	cv::namedWindow("Image 1");
	cv::imshow("Image 1",image1);
	cv::namedWindow("Image 2");
	cv::imshow("Image 2",image2);

	// Prepare the matcher
	RobustMatcher rmatcher;
	rmatcher.setConfidenceLevel(0.98);
	rmatcher.setMinDistanceToEpipolar(1.0);
	rmatcher.setRatio(0.65f);
	cv::Ptr<cv::FeatureDetector> pfd= new cv::SurfFeatureDetector(10); 
	rmatcher.setFeatureDetector(pfd);

	// Match the two images
	std::vector<cv::DMatch> matches;
	std::vector<cv::KeyPoint> keypoints1, keypoints2;
	cv::Mat fundemental= rmatcher.match(image1,image2,matches, keypoints1, keypoints2);

	// draw the matches
	cv::Mat imageMatches;
	cv::drawMatches(image1,keypoints1,  // 1st image and its keypoints
		            image2,keypoints2,  // 2nd image and its keypoints
					matches,			// the matches
					imageMatches,		// the image produced
					cv::Scalar(255,255,255)); // color of the lines
	cv::namedWindow("Matches");
	cv::imshow("Matches",imageMatches);
	
	// Convert keypoints into Point2f
	std::vector<cv::Point2f> points1, points2;
	for (std::vector<cv::DMatch>::const_iterator it= matches.begin();
		 it!= matches.end(); ++it) {

			 // Get the position of left keypoints
			 float x= keypoints1[it->queryIdx].pt.x;
			 float y= keypoints1[it->queryIdx].pt.y;
			 points1.push_back(cv::Point2f(x,y));
			 // Get the position of right keypoints
			 x= keypoints2[it->trainIdx].pt.x;
			 y= keypoints2[it->trainIdx].pt.y;
			 points2.push_back(cv::Point2f(x,y));
	}

	std::cout << points1.size() << " " << points2.size() << std::endl; 

	// Find the homography between image 1 and image 2
	std::vector<uchar> inliers(points1.size(),0);
	cv::Mat homography= cv::findHomography(
		cv::Mat(points1),cv::Mat(points2), // corresponding points
		inliers,	// outputed inliers matches 
		CV_RANSAC,	// RANSAC method
		1.);	    // max distance to reprojection point

	// Draw the inlier points
	std::vector<cv::Point2f>::const_iterator itPts= points1.begin();
	std::vector<uchar>::const_iterator itIn= inliers.begin();
	while (itPts!=points1.end()) {

		// draw a circle at each inlier location
		if (*itIn) 
 			cv::circle(image1,*itPts,3,cv::Scalar(255,255,255),2);
		
		++itPts;
		++itIn;
	}

	itPts= points2.begin();
	itIn= inliers.begin();
	while (itPts!=points2.end()) {

		// draw a circle at each inlier location
		if (*itIn) 
			cv::circle(image2,*itPts,3,cv::Scalar(255,255,255),2);
		
		++itPts;
		++itIn;
	}

    // Display the images with points
	cv::namedWindow("Image 1 Homography Points");
	cv::imshow("Image 1 Homography Points",image1);
	cv::namedWindow("Image 2 Homography Points");
	cv::imshow("Image 2 Homography Points",image2);

	// Warp image 1 to image 2
	cv::Mat result;
	cv::warpPerspective(image1, // input image
		result,			// output image
		homography,		// homography
		cv::Size(2*image1.cols,image1.rows)); // size of output image

	// Copy image 1 on the first half of full image
	cv::Mat half(result,cv::Rect(0,0,image2.cols,image2.rows));
	image2.copyTo(half);

    // Display the warp image
	cv::namedWindow("After warping");
	cv::imshow("After warping",result);

	cv::waitKey();
	return 0;
}