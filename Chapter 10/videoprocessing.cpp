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
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

#include "videoprocessor.h"

void draw(cv::Mat& img, cv::Mat& out) {

	img.copyTo(out);
	cv::circle(out, cv::Point(100,100),5,cv::Scalar(255,0,0),2);
}

void canny(cv::Mat& img, cv::Mat& out) {

	// Convert to gray
	cv::cvtColor(img,out,CV_BGR2GRAY);
	// Compute Canny edges
	cv::Canny(out,out,100,200);
	// Invert the image
	cv::threshold(out,out,128,255,cv::THRESH_BINARY_INV);
}

int main()
{
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
	int delay= 1000/rate;

	// for all frames in video
	while (!stop) {

		// read next frame if any
		if (!capture.read(frame))
			break;

		cv::imshow("Extracted Frame",frame);

		// introduce a delay
		// or press key to stop
		if (cv::waitKey(delay)>=0)
				stop= true;
	}

	// Close the video file
	capture.release();

	cv::waitKey();
		
	// Now using the VideoProcessor class

	// Create instance
	VideoProcessor processor;
	// Open video file
	processor.setInput("../bike.avi");
	// Declare a window to display the video
	processor.displayInput("Input Video");
	processor.displayOutput("Output Video");
	// Play the video at the original frame rate
	processor.setDelay(1000./processor.getFrameRate());
	// Set the frame processor callback function
	processor.setFrameProcessor(canny);
	// Start the process
	processor.run();
	cv::waitKey();
	
	// Second test
	// Create instance
    //	VideoProcessor processor;
	// Open video file
	processor.setInput("../bike.avi");

	// Get basic info about video file
	cv::Size size= processor.getFrameSize();
	std::cout << size.width << " " << size.height << std::endl;
	std::cout << processor.getFrameRate() << std::endl;
	std::cout << processor.getTotalFrameCount() << std::endl;
	std::cout << processor.getFrameNumber() << std::endl;
	std::cout << processor.getPositionMS() << std::endl;

	// No processing
	processor.dontCallProcess();
	// Output filename
//	processor.setOutput("../output/bikeOut",".jpg");
	char codec[4];
	processor.setOutput("../output/bike.avi",processor.getCodec(codec),processor.getFrameRate());
	std::cout << "Codec: " << codec[0] << codec[1] << codec[2] << codec[3] << std::endl;

	// Position the stream at frame 300
    //	processor.setFrameNumber(300);
    //	processor.stopAtFrameNo(120);

	// Declare a window to display the video
	processor.displayInput("Current Frame");
	processor.displayOutput("Output Frame");

	// Play the video at the original frame rate
	processor.setDelay(1000./processor.getFrameRate());

	// Start the process
	processor.run();

	std::cout << processor.getFrameNumber() << std::endl;
	std::cout << processor.getPositionMS() << std::endl;

	cv::waitKey();
}