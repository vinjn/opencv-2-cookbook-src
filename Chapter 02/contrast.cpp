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

#include <iostream>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>


void sharpen(const cv::Mat &image, cv::Mat &result) {

	result.create(image.size(), image.type()); // allocate if necessary

	for (int j= 1; j<image.rows-1; j++) { // for all rows (except first and last)

		const uchar* previous= image.ptr<const uchar>(j-1); // previous row
		const uchar* current= image.ptr<const uchar>(j);	// current row
		const uchar* next= image.ptr<const uchar>(j+1);		// next row

		uchar* output= result.ptr<uchar>(j);	// output row

		for (int i=1; i<image.cols-1; i++) {

			*output++= cv::saturate_cast<uchar>(5*current[i]-current[i-1]-current[i+1]-previous[i]-next[i]); 
//			output[i]= cv::saturate_cast<uchar>(5*current[i]-current[i-1]-current[i+1]-previous[i]-next[i]); 
		}
	}

	// Set the unprocess pixels to 0
	result.row(0).setTo(cv::Scalar(0));
	result.row(result.rows-1).setTo(cv::Scalar(0));
	result.col(0).setTo(cv::Scalar(0));
	result.col(result.cols-1).setTo(cv::Scalar(0));
}

void sharpen2(const cv::Mat &image, cv::Mat &result) {

	result.create(image.size(), image.type()); // allocate if necessary

	int step= image.step1();
	const uchar* previous= image.data;		// ptr to previous row
	const uchar* current=  image.data+step; // ptr to current row
	const uchar* next= image.data+2*step;   // ptr to next row
	uchar *output= result.data+step;		// ptr to output row

	for (int j= 1; j<image.rows-1; j++) { // for each row (except first and last)
		for (int i=1; i<image.cols-1; i++) { // for each column (except first and last)

			output[i]= cv::saturate_cast<uchar>(5*current[i]-current[i-1]-current[i+1]-previous[i]-next[i]); 
		}

		previous+= step;
		current+= step;
		next+= step;
		output+= step;
	}

	// Set the unprocess pixels to 0
	result.row(0).setTo(cv::Scalar(0));
	result.row(result.rows-1).setTo(cv::Scalar(0));
	result.col(0).setTo(cv::Scalar(0));
	result.col(result.cols-1).setTo(cv::Scalar(0));
}

void sharpen3(const cv::Mat &image, cv::Mat &result) {

	cv::Mat_<uchar>::const_iterator it= image.begin<uchar>()+image.step;
	cv::Mat_<uchar>::const_iterator itend= image.end<uchar>()-image.step;
	cv::Mat_<uchar>::const_iterator itup= image.begin<uchar>();
	cv::Mat_<uchar>::const_iterator itdown= image.begin<uchar>()+2*image.step;

	result.create(image.size(), image.type()); // allocate if necessary
	cv::Mat_<uchar>::iterator itout= result.begin<uchar>()+result.step;

	for ( ; it!= itend; ++it, ++itup, ++itdown) {

			*itout= cv::saturate_cast<uchar>(*it *5 - *(it-1)- *(it+1)- *itup - *itdown); 
	}
}

void sharpen2D(const cv::Mat &image, cv::Mat &result) {

	// Construct kernel (all entries initialized to 0)
	cv::Mat kernel(3,3,CV_32F,cv::Scalar(0));
	// assigns kernel values
	kernel.at<float>(1,1)= 5.0;
	kernel.at<float>(0,1)= -1.0;
	kernel.at<float>(2,1)= -1.0;
	kernel.at<float>(1,0)= -1.0;
	kernel.at<float>(1,2)= -1.0;

	//filter the image
	cv::filter2D(image,result,image.depth(),kernel);
}

int main()
{
	cv::Mat image= cv::imread("boldt.jpg",0);
	if (!image.data)
		return 0; 

	cv::Mat result;
	result.create(image.size(),image.type());

	double time= static_cast<double>(cv::getTickCount());
	sharpen(image, result);
	time= (static_cast<double>(cv::getTickCount())-time)/cv::getTickFrequency();
	std::cout << "time= " << time << std::endl;

	cv::namedWindow("Image");
	cv::imshow("Image",result);

	image= cv::imread("boldt.jpg",0);
	time= static_cast<double>(cv::getTickCount());
	sharpen3(image, result);
	time= (static_cast<double>(cv::getTickCount())-time)/cv::getTickFrequency();
	std::cout << "time 3= " << time << std::endl;

	cv::namedWindow("Image 3");
	cv::imshow("Image 3",result);

	image= cv::imread("boldt.jpg",0);
	time= static_cast<double>(cv::getTickCount());
	sharpen2D(image, result);
	time= (static_cast<double>(cv::getTickCount())-time)/cv::getTickFrequency();
	std::cout << "time 2D= " << time << std::endl;

	cv::namedWindow("Image 2D");
	cv::imshow("Image 2D",result);

	cv::waitKey();

	return 0;
}


