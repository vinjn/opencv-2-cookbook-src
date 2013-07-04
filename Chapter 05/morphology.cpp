/*------------------------------------------------------------------------------------------*\
   This file contains material supporting chapter 5 of the cookbook:  
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
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

int main()
{
	// Read input image
	cv::Mat image= cv::imread("../binary.bmp");
	if (!image.data)
		return 0; 

    // Display the image
	cv::namedWindow("Image");
	cv::imshow("Image",image);

	// Erode the image
	cv::Mat eroded;
	cv::erode(image,eroded,cv::Mat());

    // Display the eroded image
	cv::namedWindow("Eroded Image");
	cv::imshow("Eroded Image",eroded);

	// Dilate the image
	cv::Mat dilated;
	cv::dilate(image,dilated,cv::Mat());

    // Display the dialted image
	cv::namedWindow("Dilated Image");
	cv::imshow("Dilated Image",dilated);

	// Erode the image with a larger s.e.
	cv::Mat element(7,7,CV_8U,cv::Scalar(1));
	cv::erode(image,eroded,element);

    // Display the eroded image
	cv::namedWindow("Eroded Image (7x7)");
	cv::imshow("Eroded Image (7x7)",eroded);

	// Erode the image 3 times.
	cv::erode(image,eroded,cv::Mat(),cv::Point(-1,-1),3);

    // Display the eroded image
	cv::namedWindow("Eroded Image (3 times)");
	cv::imshow("Eroded Image (3 times)",eroded);

	// Close the image
	cv::Mat element5(5,5,CV_8U,cv::Scalar(1));
	cv::Mat closed;
	cv::morphologyEx(image,closed,cv::MORPH_CLOSE,element5);

    // Display the opened image
	cv::namedWindow("Closed Image");
	cv::imshow("Closed Image",closed);

	// Open the image
	cv::Mat opened;
	cv::morphologyEx(image,opened,cv::MORPH_OPEN,element5);

    // Display the opened image
	cv::namedWindow("Opened Image");
	cv::imshow("Opened Image",opened);

	// Close and Open the image
	cv::morphologyEx(image,image,cv::MORPH_CLOSE,element5);
	cv::morphologyEx(image,image,cv::MORPH_OPEN,element5);

    // Display the close/opened image
	cv::namedWindow("Closed and Opened Image");
	cv::imshow("Closed and Opened Image",image);
	cv::imwrite("binaryGroup.bmp",image);

	// Read input image
	image= cv::imread("../binary.bmp");

	// Open and Close the image
	cv::morphologyEx(image,image,cv::MORPH_OPEN,element5);
	cv::morphologyEx(image,image,cv::MORPH_CLOSE,element5);

    // Display the close/opened image
	cv::namedWindow("Opened and Closed Image");
	cv::imshow("Opened and Closed Image",image);

	cv::waitKey();
	return 0;
}

