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

#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

// function that creates and returns an image
cv::Mat function() {

	// create image
	cv::Mat ima(240,320,CV_8U,cv::Scalar(100));
	// return it
	return ima;
}

int main() {

	// create image
	cv::Mat image;
	// print image size
	std::cout << "size: " << image.size().height << " , " 
          << image.size().width << std::endl;
	// open image
	image=  cv::imread("img.jpg");
	// check if image has been successfully read
	if (!image.data) { 
		// no image has been created…
		return 0;
	}
	// print image size
    std::cout << "size (after reading): " << image.size().height << " , " 
          << image.size().width << std::endl;

	// display image
	cv::namedWindow("Original Image"); // define the window
    cv::imshow("Original Image", image); // show the image

	// create another image
	cv::Mat result;
	// flip the image
	cv::flip(image,result,1); // positive for horizontal
                              // 0 for vertical, 						  
                              // negative for both
	// display result
	cv::namedWindow("Output Image");
	cv::imshow("Output Image", result);
	// wait for key pressed
	cv::waitKey(0);
	// write image on file
	cv::imwrite("output.bmp", result);

	// create two new images
	cv::Mat image2, image3;

	image2= result; // the two images refer to the same data
	result.copyTo(image3); // a new copy is created

	// flip vertically this time
	cv::flip(result,result,0); 

	// display result
	cv::namedWindow("image 2");
	cv::imshow("image 2", image2);
	cv::namedWindow("image 3");
	cv::imshow("image 3", image3);

	// get a gray-level image
	cv::Mat gray= function();
	// display result
	cv::namedWindow("Gray Image");
	cv::imshow("Gray Image", gray);

	// wait for key pressed
	cv::waitKey(0);
	return 1;
}

