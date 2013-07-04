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

#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/features2d/features2d.hpp>
#include <opencv2/video/tracking.hpp>

#include "featuretracker.h"

int main()
{
	// Create video procesor instance
	VideoProcessor processor;

	// Create feature tracker instance
	FeatureTracker tracker;

	// Open video file
	processor.setInput("../bike.avi");

	// set frame processor
	processor.setFrameProcessor(&tracker);

	// Declare a window to display the video
	processor.displayOutput("Tracked Features");

	// Play the video at the original frame rate
	processor.setDelay(1000./processor.getFrameRate());

	// Start the process
	processor.run();

	cv::waitKey();
	/*
		// Open the video file
    cv::VideoCapture capture("../bike.avi");
	// check if video successfully opened
	if (!capture.isOpened())
		return 1;

	// Get the frame rate
	double rate= capture.get(CV_CAP_PROP_FPS);

	bool stop(false);
	cv::Mat frame; // current video frame
	cv::namedWindow("Extracted Frame");

	// Delay between each frame
	// corresponds to video frame rate
	int delay= 1000/rate/2;



	cv::Mat gray,gray_prev;
	std::vector<cv::Point2f> points[2];
	std::vector<cv::Point2f> features;
	const int MAX_COUNT = 500;
	cv::Size winSize(10,10);
	cv::TermCriteria termcrit(CV_TERMCRIT_ITER|CV_TERMCRIT_EPS,20,0.03);
	bool firstframe =true;



	// for all frames in video
	while (!stop) {

		// read next frame if any
		if (!capture.read(frame))
			break;


		cv::cvtColor(frame, gray, CV_BGR2GRAY); 
		if(firstframe)
		{
			cv::goodFeaturesToTrack(gray, features, MAX_COUNT, 0.01, 10, cv::Mat(), 3, 0, 0.04);
            cv::cornerSubPix(gray, features, winSize, cv::Size(-1,-1), termcrit);
			points[0].insert(points[0].end(),features.begin(),features.end());
			firstframe=false;
		}
		
		{
			std::vector<uchar> status;
            std::vector<float> err;
			if(gray_prev.empty())
                gray.copyTo(gray_prev);
            cv::calcOpticalFlowPyrLK(gray_prev, gray, points[0], points[1], status, err, winSize,3, termcrit, 0);
            size_t i, k;
            for( i = k = 0; i < points[1].size(); i++ )
            {
                if( !status[i] )
                    continue;
				if ((abs(points[0][i].x-points[1][i].x)+(abs(points[0][i].y-points[1][i].y))<2))
					continue;

                cv::line( frame, points[0][i],points[1][i], cv::Scalar(0,255,0));
                points[1][k++] = points[1][i];
                cv::circle( frame, points[1][i], 3, cv::Scalar(0,255,0), -1, 8);
            }
            points[1].resize(k);
		}
		if(points[1].size()<=10)
			firstframe=true;
		std::swap(points[1], points[0]);
        cv::swap(gray_prev, gray);


		cv::imshow("Extracted Frame",frame);

		// introduce a delay
		// or press key to stop
		if (cv::waitKey(delay)>=0)
				stop= true;
	}

	// Close the video file
	capture.release();

	cv::waitKey();

	
	// Create instance
	VideoProcessor processor;
	// Open video file
	processor.setInput("../bike.avi");
	// Output filename
//	processor.setOutput("../output/bikeOut",".jpg");

	// Declare a window to display the video
	processor.displayInput("Current Frame");
	processor.displayOutput("Output Frame");

	// Play the video at the original frame rate
	processor.setDelay(1000./processor.getFrameRate());

	// Set the frame processor callback function
	processor.setFrameProcessor(draw);

	// Start the process
	processor.run();

	cv::waitKey(5000);*/
}