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
#include "morphoFeatures.h"

int main()
{
	// Read input image
	cv::Mat image= cv::imread("../building.jpg",0);
	if (!image.data)
		return 0; 

    // Display the image
	cv::namedWindow("Image");
	cv::imshow("Image",image);

	// Create the morphological features instance
	MorphoFeatures morpho;
	morpho.setThreshold(40);

	// Get the edges
	cv::Mat edges;
	edges= morpho.getEdges(image);

    // Display the edge image
	cv::namedWindow("Edge Image");
	cv::imshow("Edge Image",edges);

	// Get the corners
    morpho.setThreshold(-1);
	cv::Mat corners;
	corners= morpho.getCorners(image);
	cv::morphologyEx(corners,corners,cv::MORPH_TOPHAT,cv::Mat());
    cv::threshold(corners, corners, 40, 255, cv::THRESH_BINARY_INV);

    // Display the corner image
	cv::namedWindow("Corner Image");
	cv::imshow("Corner Image",corners);

    // Display the corner on the image
	morpho.drawOnImage(corners,image);
	cv::namedWindow("Corners on Image");
	cv::imshow("Corners on Image",image);

	// Read and display image of square
	image= cv::imread("../square.bmp",0);
	cv::namedWindow("Square Image");
	cv::imshow("Square Image",image);

	// Creating the cross-shaped structuring element
	cv::Mat cross(5,5,CV_8U,cv::Scalar(0));
	for (int i=0; i<5; i++) {
		  
	  cross.at<uchar>(2,i)= 1;
	  cross.at<uchar>(i,2)= 1;									
	}
		  
	// Dilate with a cross	
	cv::Mat result;
	cv::dilate(image,result,cross);

	// Display the result
	cv::namedWindow("Dilated square with cross");
	cv::imshow("Dilated square with cross",result);

	// Creating the diamond-shaped structuring element
	cv::Mat diamond(5,5,CV_8U,cv::Scalar(1));
	diamond.at<uchar>(0,0)= 0;
	diamond.at<uchar>(0,1)= 0;
	diamond.at<uchar>(1,0)= 0;
	diamond.at<uchar>(4,4)= 0;
	diamond.at<uchar>(3,4)= 0;
	diamond.at<uchar>(4,3)= 0;
	diamond.at<uchar>(4,0)= 0;
	diamond.at<uchar>(4,1)= 0;
	diamond.at<uchar>(3,0)= 0;
	diamond.at<uchar>(0,4)= 0;
	diamond.at<uchar>(0,3)= 0;
	diamond.at<uchar>(1,4)= 0;

	// Erode with a diamond
	cv::Mat result2;
	cv::erode(result,result2,diamond);

	// Display the result
	cv::namedWindow("Eroded square with diamond");
	cv::imshow("Eroded square with diamond",result2);

	// Combine the images into one
	cv::Mat final(100,300,CV_8U);
	cv::Mat window= final(cv::Rect(0,0,100,100));
	image.copyTo(window);
	window= final(cv::Rect(100,0,100,100));
	result.copyTo(window);
	window= final(cv::Rect(200,0,100,100));
	result2.copyTo(window);

	// Display the combined result
	cv::namedWindow("Combined");
	cv::imshow("Combined",final);

	// Save combined result
	cv::imwrite("squares.bmp",final);

	cv::waitKey();

	return 0;
}
