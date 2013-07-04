/*------------------------------------------------------------------------------------------*\
   This file contains material supporting chapter 10 of the cookbook:  
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

#if !defined FTRACKER
#define FTRACKER

#include <string>
#include <vector>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/features2d/features2d.hpp>
#include <opencv2/video/tracking.hpp>

#include "videoprocessor.h"

class FeatureTracker : public FrameProcessor {
	
	cv::Mat gray;			// current gray-level image
	cv::Mat gray_prev;		// previous gray-level image
	std::vector<cv::Point2f> points[2]; // tracked features from 0->1
	std::vector<cv::Point2f> initial;   // initial position of tracked points
	std::vector<cv::Point2f> features;  // detected features
	int max_count;	  // maximum number of features to detect
	double qlevel;    // quality level for feature detection
	double minDist;   // minimum distance between two feature points
	std::vector<uchar> status; // status of tracked features
    std::vector<float> err;    // error in tracking

  public:

	FeatureTracker() : max_count(500), qlevel(0.01), minDist(10.) {}
	
	// processing method
	void process(cv:: Mat &frame, cv:: Mat &output) {

		// convert to gray-level image
		cv::cvtColor(frame, gray, CV_BGR2GRAY); 
		frame.copyTo(output);

		// 1. if new feature points must be added
		if(addNewPoints())
		{
			// detect feature points
			detectFeaturePoints();
			// add the detected features to the currently tracked features
			points[0].insert(points[0].end(),features.begin(),features.end());
			initial.insert(initial.end(),features.begin(),features.end());
		}
		
		// for first image of the sequence
		if(gray_prev.empty())
           gray.copyTo(gray_prev);
            
		// 2. track features
		cv::calcOpticalFlowPyrLK(gray_prev, gray, // 2 consecutive images
			points[0], // input point position in first image
			points[1], // output point postion in the second image
			status,    // tracking success
			err);      // tracking error
           
		// 2. loop over the tracked points to reject the undesirables
		int k=0;
		for( int i= 0; i < points[1].size(); i++ ) {

			// do we keep this point?
			if (acceptTrackedPoint(i)) {

				// keep this point in vector
				initial[k]= initial[i];
				points[1][k++] = points[1][i];
			}
		}

		// eliminate unsuccesful points
        points[1].resize(k);
		initial.resize(k);

		// 3. handle the accepted tracked points
		handleTrackedPoints(frame, output);

		// 4. current points and image become previous ones
		std::swap(points[1], points[0]);
        cv::swap(gray_prev, gray);
	}

	// feature point detection
	void detectFeaturePoints() {
			
		// detect the features
		cv::goodFeaturesToTrack(gray, // the image 
			features,   // the output detected features
			max_count,  // the maximum number of features 
			qlevel,     // quality level
			minDist);   // min distance between two features
	}

	// determine if new points should be added
	bool addNewPoints() {

		// if too few points
		return points[0].size()<=10;
	}

	// determine which tracked point should be accepted
	bool acceptTrackedPoint(int i) {

		return status[i] &&
			// if point has moved
			(abs(points[0][i].x-points[1][i].x)+
			(abs(points[0][i].y-points[1][i].y))>2);
	}

	// handle the currently tracked points
	void handleTrackedPoints(cv:: Mat &frame, cv:: Mat &output) {

		// for all tracked points
		for(int i= 0; i < points[1].size(); i++ ) {

			// draw line and circle
		    cv::line(output, initial[i], points[1][i], cv::Scalar(255,255,255));
			cv::circle(output, points[1][i], 3, cv::Scalar(255,255,255),-1);
		}
	}
};

#endif
