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

int main()
{
	// Read input image
	cv::Mat image= cv::imread("../group.jpg",0);
	if (!image.data)
		return 0; 

    // Display the image
	cv::namedWindow("Image");
	cv::imshow("Image",image);

	// The histogram object
	Histogram1D h;

    // Compute the histogram
	cv::MatND histo= h.getHistogram(image);

	// Loop over each bin
	for (int i=0; i<256; i++) 
		cout << "Value " << i << " = " << histo.at<float>(i) << endl;  

	// Display a histogram as an image
	cv::namedWindow("Histogram");
	cv::imshow("Histogram",h.getHistogramImage(image));

	// creating a binary image by thresholding at the valley
	cv::Mat thresholded;
	cv::threshold(image,thresholded,60,255,cv::THRESH_BINARY);
 
	// Display the thresholded image
	cv::namedWindow("Binary Image");
	cv::imshow("Binary Image",thresholded);
	cv::imwrite("binary.bmp",thresholded);

	// Equalize the image
	cv::Mat eq= h.equalize(image);

	// Show the result
	cv::namedWindow("Equalized Image");
	cv::imshow("Equalized Image",eq);

	// Show the new histogram
	cv::namedWindow("Equalized Histogram");
	cv::imshow("Equalized Histogram",h.getHistogramImage(eq));

	// Stretch the image ignoring bins with less than 5 pixels
	cv::Mat str= h.stretch(image,5);

	// Show the result
	cv::namedWindow("Stretched Image");
	cv::imshow("Stretched Image",str);

	// Show the new histogram
	cv::namedWindow("Stretched Histogram");
	cv::imshow("Stretched Histogram",h.getHistogramImage(str));

	// Create an image inversion table
	uchar lookup[256];
	
	for (int i=0; i<256; i++) {
		
		lookup[i]= 255-i;
	}

	// Apply lookup and display negative image
	cv::namedWindow("Negative image");
	cv::imshow("Negative image",h.applyLookUp(image,lookup));

	cv::waitKey();
	return 0;
}

