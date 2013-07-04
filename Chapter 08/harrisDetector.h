/*------------------------------------------------------------------------------------------*\
   This file contains material supporting chapter 8 of the cookbook:  
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

#if !defined HARRISD
#define HARRISD

#include <vector>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/features2d/features2d.hpp>

class HarrisDetector {

  private:

	  // 32-bit float image of corner strength
	  cv::Mat cornerStrength;
	  // 32-bit float image of thresholded corners
	  cv::Mat cornerTh;
	  // image of local maxima (internal)
	  cv::Mat localMax;
	  // size of neighbourhood for derivatives smoothing
	  int neighbourhood; 
	  // aperture for gradient computation
	  int aperture; 
	  // Harris parameter
	  double k;
	  // maximum strength for threshold computation
	  double maxStrength;
	  // calculated threshold (internal)
	  double threshold;
	  // size of neighbourhood for non-max suppression
	  int nonMaxSize; 
	  // kernel for non-max suppression
	  cv::Mat kernel;

  public:

	  HarrisDetector() : neighbourhood(3), aperture(3), k(0.1), maxStrength(0.0), threshold(0.01), nonMaxSize(3) {
	  
		  setLocalMaxWindowSize(nonMaxSize);
	  }

	  // Create kernel used in non-maxima suppression
	  void setLocalMaxWindowSize(int size) {

		  nonMaxSize= size;
		  kernel.create(nonMaxSize,nonMaxSize,CV_8U);
	  }

	  // Compute Harris corners
	  void detect(const cv::Mat& image) {
	
		  // Harris computation
		  cv::cornerHarris(image,cornerStrength,
		             neighbourhood,// neighborhood size
					 aperture,     // aperture size
					 k);           // Harris parameter
	
		  // internal threshold computation
		  double minStrength; // not used
		  cv::minMaxLoc(cornerStrength,&minStrength,&maxStrength);

		  // local maxima detection
		  cv::Mat dilated;  // temporary image
		  cv::dilate(cornerStrength,dilated,cv::Mat());
		  cv::compare(cornerStrength,dilated,localMax,cv::CMP_EQ);
	  }

	  // Get the corner map from the computed Harris values
	  cv::Mat getCornerMap(double qualityLevel) {

		  cv::Mat cornerMap;

		  // thresholding the corner strength
		  threshold= qualityLevel*maxStrength;
		  cv::threshold(cornerStrength,cornerTh,threshold,255,cv::THRESH_BINARY);

		  // convert to 8-bit image
		  cornerTh.convertTo(cornerMap,CV_8U);
	
		  // non-maxima suppression
		  cv::bitwise_and(cornerMap,localMax,cornerMap);

		  return cornerMap;
	  }

	  // Get the feature points vector from the computed Harris values
	  void getCorners(std::vector<cv::Point> &points, double qualityLevel) {

		  // Get the corner map
		  cv::Mat cornerMap= getCornerMap(qualityLevel);
		  // Get the corners
		  getCorners(points, cornerMap);
	  }

	  // Get the feature points vector from the computed corner map
	  void getCorners(std::vector<cv::Point> &points, const cv::Mat& cornerMap) {
			  
		  // Iterate over the pixels to obtain all feature points
		  for( int y = 0; y < cornerMap.rows; y++ ) {
    
			  const uchar* rowPtr = cornerMap.ptr<uchar>(y);
    
			  for( int x = 0; x < cornerMap.cols; x++ ) {

				  // if it is a feature point
				  if (rowPtr[x]) {

					  points.push_back(cv::Point(x,y));
				  }
			  } 
		  }
	  }

	  // Draw circles at feature point locations on an image
	  void drawOnImage(cv::Mat &image, const std::vector<cv::Point> &points, cv::Scalar color= cv::Scalar(255,255,255), int radius=3, int thickness=2) {

		  std::vector<cv::Point>::const_iterator it= points.begin();

		  // for all corners
		  while (it!=points.end()) {

			  // draw a circle at each corner location
			  cv::circle(image,*it,radius,color,thickness);
			  ++it;
		  }
	  }
};

#endif
