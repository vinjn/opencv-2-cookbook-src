/*------------------------------------------------------------------------------------------*\
   This file contains material supporting chapter 2 of the cookbook:  
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

void salt(cv::Mat &image, int n) {

	int i,j;
	for (int k=0; k<n; k++) {

		// rand() is the MFC random number generator
		i= rand()%image.cols;
		j= rand()%image.rows;


		if (image.channels() == 1) { // gray-level image

			image.at<uchar>(j,i)= 255; 

		} else if (image.channels() == 3) { // color image

			image.at<cv::Vec3b>(j,i)[0]= 255; 
			image.at<cv::Vec3b>(j,i)[1]= 255; 
			image.at<cv::Vec3b>(j,i)[2]= 255; 
		}
	}
}

int main()
{
	srand(cv::getTickCount()); // init random number generator

	cv::Mat image= cv::imread("boldt.jpg",0);

	salt(image,3000);

	cv::namedWindow("Image");
	cv::imshow("Image",image);

	cv::imwrite("salted.bmp",image);

	cv::waitKey(5000);

	return 0;
}


