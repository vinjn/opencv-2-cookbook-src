/*------------------------------------------------------------------------------------------*\
   This file contains material supporting chapter 1 of the cookbook:  
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

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

int main() {

	// read an image
	cv::Mat image= cv::imread("img.jpg");	
	// create image window named "My Image"
	cv::namedWindow("My Image");
	// show the image on window
	cv::imshow("My Image", image);
	// wait key for 5000 ms
	cv::waitKey(5000);

	return 1;
}
