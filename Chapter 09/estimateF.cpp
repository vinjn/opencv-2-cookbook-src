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

int main()
{
	// Read input images
	cv::Mat image1= cv::imread("../church01.jpg",0);
	cv::Mat image2= cv::imread("../church03.jpg",0);
	if (!image1.data || !image2.data)
		return 0; 

    // Display the images
	cv::namedWindow("Right Image");
	cv::imshow("Right Image",image1);
	cv::namedWindow("Left Image");
	cv::imshow("Left Image",image2);

	// vector of keypoints
	std::vector<cv::KeyPoint> keypoints1;
	std::vector<cv::KeyPoint> keypoints2;

	// Construction of the SURF feature detector 
	cv::SurfFeatureDetector surf(3000);

	// Detection of the SURF features
	surf.detect(image1,keypoints1);
	surf.detect(image2,keypoints2);

	std::cout << "Number of SURF points (1): " << keypoints1.size() << std::endl;
	std::cout << "Number of SURF points (2): " << keypoints2.size() << std::endl;
	
	// Draw the kepoints
	cv::Mat imageKP;
	cv::drawKeypoints(image1,keypoints1,imageKP,cv::Scalar(255,255,255),cv::DrawMatchesFlags::DRAW_RICH_KEYPOINTS);
	cv::namedWindow("Right SURF Features");
	cv::imshow("Right SURF Features",imageKP);
	cv::drawKeypoints(image2,keypoints2,imageKP,cv::Scalar(255,255,255),cv::DrawMatchesFlags::DRAW_RICH_KEYPOINTS);
	cv::namedWindow("Left SURF Features");
	cv::imshow("Left SURF Features",imageKP);

	// Construction of the SURF descriptor extractor 
	cv::SurfDescriptorExtractor surfDesc;

	// Extraction of the SURF descriptors
	cv::Mat descriptors1, descriptors2;
	surfDesc.compute(image1,keypoints1,descriptors1);
	surfDesc.compute(image2,keypoints2,descriptors2);

	std::cout << "descriptor matrix size: " << descriptors1.rows << " by " << descriptors1.cols << std::endl;

	// Construction of the matcher 
	cv::BruteForceMatcher<cv::L2<float>> matcher;

	// Match the two image descriptors
	std::vector<cv::DMatch> matches;
	matcher.match(descriptors1,descriptors2, matches);

	std::cout << "Number of matched points: " << matches.size() << std::endl;

	// Select few Matches  
	std::vector<cv::DMatch> selMatches;
	/*
	keypoints1.push_back(cv::KeyPoint(342.,615.,2));  
	keypoints2.push_back(cv::KeyPoint(410.,600.,2));
	selMatches.push_back(cv::DMatch(keypoints1.size()-1,keypoints2.size()-1,0)); // street light bulb
	selMatches.push_back(matches[6]);  // right tower
	selMatches.push_back(matches[60]);  // left bottom window
	selMatches.push_back(matches[139]);
	selMatches.push_back(matches[141]);  // middle window
	selMatches.push_back(matches[213]);
	selMatches.push_back(matches[273]);
	
	int kk=0;
	while (kk<matches.size()) {
		std::cout<<kk<<std::endl;
	selMatches.push_back(matches[kk++]); 
	selMatches.pop_back();
	cv::waitKey();
	}
	*/

	/* between church01 and church03 */
	selMatches.push_back(matches[14]);  
	selMatches.push_back(matches[16]);
	selMatches.push_back(matches[141]);  
	selMatches.push_back(matches[146]);
	selMatches.push_back(matches[235]);
	selMatches.push_back(matches[238]);
	selMatches.push_back(matches[274]);

	// Draw the selected matches
	cv::Mat imageMatches;
	cv::drawMatches(image1,keypoints1,  // 1st image and its keypoints
		            image2,keypoints2,  // 2nd image and its keypoints
//					selMatches,			// the matches
					matches,			// the matches
					imageMatches,		// the image produced
					cv::Scalar(255,255,255)); // color of the lines
	cv::namedWindow("Matches");
	cv::imshow("Matches",imageMatches);

	// Convert 1 vector of keypoints into
	// 2 vectors of Point2f
	std::vector<int> pointIndexes1;
	std::vector<int> pointIndexes2;
	for (std::vector<cv::DMatch>::const_iterator it= selMatches.begin();
		 it!= selMatches.end(); ++it) {

			 // Get the indexes of the selected matched keypoints
			 pointIndexes1.push_back(it->queryIdx);
			 pointIndexes2.push_back(it->trainIdx);
	}
		 
	// Convert keypoints into Point2f
	std::vector<cv::Point2f> selPoints1, selPoints2;
	cv::KeyPoint::convert(keypoints1,selPoints1,pointIndexes1);
	cv::KeyPoint::convert(keypoints2,selPoints2,pointIndexes2);

	// check by drawing the points 
	std::vector<cv::Point2f>::const_iterator it= selPoints1.begin();
	while (it!=selPoints1.end()) {

		// draw a circle at each corner location
		cv::circle(image1,*it,3,cv::Scalar(255,255,255),2);
		++it;
	}

	it= selPoints2.begin();
	while (it!=selPoints2.end()) {

		// draw a circle at each corner location
		cv::circle(image2,*it,3,cv::Scalar(255,255,255),2);
		++it;
	}

	// Compute F matrix from 7 matches
	cv::Mat fundemental= cv::findFundamentalMat(
		cv::Mat(selPoints1), // points in first image
		cv::Mat(selPoints2), // points in second image
		CV_FM_7POINT);       // 7-point method

	std::cout << "F-Matrix size= " << fundemental.rows << "," << fundemental.cols << std::endl;  

	// draw the left points corresponding epipolar lines in right image 
	std::vector<cv::Vec3f> lines1; 
	cv::computeCorrespondEpilines(
		cv::Mat(selPoints1), // image points 
		1,                   // in image 1 (can also be 2)
		fundemental, // F matrix
		lines1);     // vector of epipolar lines

	// for all epipolar lines
	for (vector<cv::Vec3f>::const_iterator it= lines1.begin();
		 it!=lines1.end(); ++it) {

			 // draw the epipolar line between first and last column
			 cv::line(image2,cv::Point(0,-(*it)[2]/(*it)[1]),
				             cv::Point(image2.cols,-((*it)[2]+(*it)[0]*image2.cols)/(*it)[1]),
							 cv::Scalar(255,255,255));
	}
		
	// draw the left points corresponding epipolar lines in left image 
	std::vector<cv::Vec3f> lines2;
	cv::computeCorrespondEpilines(cv::Mat(selPoints2),2,fundemental,lines2);
	for (vector<cv::Vec3f>::const_iterator it= lines2.begin();
		 it!=lines2.end(); ++it) {

			 // draw the epipolar line between first and last column
			 cv::line(image1,cv::Point(0,-(*it)[2]/(*it)[1]),
				             cv::Point(image1.cols,-((*it)[2]+(*it)[0]*image1.cols)/(*it)[1]),
							 cv::Scalar(255,255,255));
	}
		
    // Display the images with points and epipolar lines
	cv::namedWindow("Right Image Epilines");
	cv::imshow("Right Image Epilines",image1);
	cv::namedWindow("Left Image Epilines");
	cv::imshow("Left Image Epilines",image2);

	/*
	std::nth_element(matches.begin(),    // initial position
		             matches.begin()+matches.size()/2, // 50%
					 matches.end());     // end position
	// remove all elements after the 
	matches.erase(matches.begin()+matches.size()/2, matches.end()); 
*/
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

	// Compute F matrix using RANSAC
	std::vector<uchar> inliers(points1.size(),0);
	fundemental= cv::findFundamentalMat(
		cv::Mat(points1),cv::Mat(points2), // matching points
		inliers,      // match status (inlier ou outlier)  
		CV_FM_RANSAC, // RANSAC method
		1,            // distance to epipolar line
		0.98);        // confidence probability

	// Read input images
	image1= cv::imread("../church01.jpg",0);
	image2= cv::imread("../church03.jpg",0);

	// Draw the epipolar line of few points
	cv::computeCorrespondEpilines(cv::Mat(selPoints1),1,fundemental,lines1);
	for (vector<cv::Vec3f>::const_iterator it= lines1.begin();
		 it!=lines1.end(); ++it) {

			 cv::line(image2,cv::Point(0,-(*it)[2]/(*it)[1]),
				             cv::Point(image2.cols,-((*it)[2]+(*it)[0]*image2.cols)/(*it)[1]),
							 cv::Scalar(255,255,255));
	}

	cv::computeCorrespondEpilines(cv::Mat(selPoints2),2,fundemental,lines2);
	for (vector<cv::Vec3f>::const_iterator it= lines2.begin();
		 it!=lines2.end(); ++it) {

			 cv::line(image1,cv::Point(0,-(*it)[2]/(*it)[1]),
				             cv::Point(image1.cols,-((*it)[2]+(*it)[0]*image1.cols)/(*it)[1]),
							 cv::Scalar(255,255,255));
	}

	// Draw the inlier points
	std::vector<cv::Point2f> points1In, points2In;
	std::vector<cv::Point2f>::const_iterator itPts= points1.begin();
	std::vector<uchar>::const_iterator itIn= inliers.begin();
	while (itPts!=points1.end()) {

		// draw a circle at each inlier location
		if (*itIn) {
 			cv::circle(image1,*itPts,3,cv::Scalar(255,255,255),2);
			points1In.push_back(*itPts);
		}
		++itPts;
		++itIn;
	}

	itPts= points2.begin();
	itIn= inliers.begin();
	while (itPts!=points2.end()) {

		// draw a circle at each inlier location
		if (*itIn) {
			cv::circle(image2,*itPts,3,cv::Scalar(255,255,255),2);
			points2In.push_back(*itPts);
		}
		++itPts;
		++itIn;
	}

    // Display the images with points
	cv::namedWindow("Right Image Epilines (RANSAC)");
	cv::imshow("Right Image Epilines (RANSAC)",image1);
	cv::namedWindow("Left Image Epilines (RANSAC)");
	cv::imshow("Left Image Epilines (RANSAC)",image2);

	cv::findHomography(cv::Mat(points1In),cv::Mat(points2In),inliers,CV_RANSAC,1.);

	// Read input images
	image1= cv::imread("../church01.jpg",0);
	image2= cv::imread("../church03.jpg",0);

	// Draw the inlier points
	itPts= points1In.begin();
	itIn= inliers.begin();
	while (itPts!=points1In.end()) {

		// draw a circle at each inlier location
		if (*itIn) 
 			cv::circle(image1,*itPts,3,cv::Scalar(255,255,255),2);
		
		++itPts;
		++itIn;
	}

	itPts= points2In.begin();
	itIn= inliers.begin();
	while (itPts!=points2In.end()) {

		// draw a circle at each inlier location
		if (*itIn) 
			cv::circle(image2,*itPts,3,cv::Scalar(255,255,255),2);
		
		++itPts;
		++itIn;
	}

    // Display the images with points
	cv::namedWindow("Right Image Homography (RANSAC)");
	cv::imshow("Right Image Homography (RANSAC)",image1);
	cv::namedWindow("Left Image Homography (RANSAC)");
	cv::imshow("Left Image Homography (RANSAC)",image2);

	cv::waitKey();
	return 0;
	}