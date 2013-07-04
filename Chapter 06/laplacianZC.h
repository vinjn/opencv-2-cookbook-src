/*------------------------------------------------------------------------------------------*\
   This file contains material supporting chapter 6 of the cookbook:  
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

#if !defined LAPLACEZC
#define LAPLACEZC

#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>

class LaplacianZC {

  private:

	  // original image
	  cv::Mat img;

	  // 32-bit float image containing the Laplacian
	  cv::Mat laplace;
	  // Aperture size of the laplacian kernel
	  int aperture;

  public:

	  LaplacianZC() : aperture(3) {}

	  // Set the aperture size of the kernel
	  void setAperture(int a) {

		  aperture= a;
	  }

	  // Get the aperture size of the kernel
	  int getAperture() const {

		  return aperture;
	  }

	  // Compute the floating point Laplacian
	  cv::Mat computeLaplacian(const cv::Mat& image) {


		  // Compute Laplacian
		  cv::Laplacian(image,laplace,CV_32F,aperture);

		  // Keep local copy of the image
		  // (used for zero-crossings)
		  img= image.clone();

		  return laplace;
	  }

	  // Get the Laplacian result in 8-bit image 
	  // zero corresponds to gray level 128
	  // if no scale is provided, then the max value will be
	  // scaled to intensity 255
	  // You must call computeLaplacian before calling this method
	  cv::Mat getLaplacianImage(double scale=-1.0) {

		  if (scale<0) {
	
			  double lapmin, lapmax;
			  cv::minMaxLoc(laplace,&lapmin,&lapmax);

			  scale= 127/ std::max(-lapmin,lapmax);
		  }

		  cv::Mat laplaceImage;
		  laplace.convertTo(laplaceImage,CV_8U,scale,128);

		  return laplaceImage;
	  }

	  // Get a binary image of the zero-crossings
	  // if the product of the two adjascent pixels is
	  // less than threshold then this zero-crossing will be ignored
	  cv::Mat getZeroCrossings(float threshold=1.0) {

		  // Create the iterators
		  cv::Mat_<float>::const_iterator it= laplace.begin<float>()+laplace.step1();
		  cv::Mat_<float>::const_iterator itend= laplace.end<float>();
		  cv::Mat_<float>::const_iterator itup= laplace.begin<float>();

		  // Binary image initialize to white
		  cv::Mat binary(laplace.size(),CV_8U,cv::Scalar(255));
		  cv::Mat_<uchar>::iterator itout= binary.begin<uchar>()+binary.step1();

		  // negate the input threshold value
		  threshold *= -1.0;

		  for ( ; it!= itend; ++it, ++itup, ++itout) {

			  // if the product of two adjascent pixel is negative
			  // then there is a sign change
			  if (*it * *(it-1) < threshold)
				  *itout= 0; // horizontal zero-crossing
			  else if (*it * *itup < threshold)
				  *itout= 0; // vertical zero-crossing
		  }

		  return binary;
	  }

	  // Get a binary image of the zero-crossings
	  // if the product of the two adjacent pixels is
	  // less than threshold then this zero-crossing will be ignored
	  cv::Mat getZeroCrossingsWithSobel(float threshold) {

		  cv::Mat sx;
		  cv::Sobel(img,sx,CV_32F,1,0,1);
		  cv::Mat sy;
		  cv::Sobel(img,sy,CV_32F,0,1,1);

		  // Create the iterators
		  cv::Mat_<float>::const_iterator it= laplace.begin<float>()+laplace.step1();
		  cv::Mat_<float>::const_iterator itend= laplace.end<float>();
		  cv::Mat_<float>::const_iterator itup= laplace.begin<float>();
		  cv::Mat_<float>::const_iterator itx= sx.begin<float>()+sx.step1();
		  cv::Mat_<float>::const_iterator ity= sy.begin<float>()+sy.step1();

		  // Binary image initialize to white
		  cv::Mat binary(laplace.size(),CV_8U,cv::Scalar(255));
		  cv::Mat_<uchar>::iterator itout= binary.begin<uchar>()+binary.step1();

		  for ( ; it!= itend; ++it, ++itup, ++itout, ++itx, ++ity) {

			  // if the product of two adjacent pixel is negative
			  // then there is a sign change
			  if (*it * *(it-1) < 0.0 && fabs(*ity) > threshold)
				  *itout= 0; // horizontal zero-crossing
			  else if (*it * *itup < 0.0 && fabs(*ity) > threshold)
				  *itout= 0; // vertical zero-crossing
		  }

		  return binary;
	  }

};


#endif
