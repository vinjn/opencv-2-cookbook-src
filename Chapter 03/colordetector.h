/*------------------------------------------------------------------------------------------*\
   This file contains material supporting chapter 3 of the cookbook:  
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

#if !defined COLORDETECT
#define COLORDETECT

#include <opencv2/core/core.hpp>

class ColorDetector {

  private:

	  // minimum acceptable distance
	  int minDist; 

	  // target color
	  cv::Vec3b target; 

	  // image containing resulting binary map
	  cv::Mat result;

	  // inline private member function
	  // Computes the distance from target color.
	  int getDistance(const cv::Vec3b& color) const {
		 // return static_cast<int>(cv::norm<int,3>(cv::Vec3i(color[0]-target[0],color[1]-target[1],color[2]-target[2])));
		  return abs(color[0]-target[0])+
					abs(color[1]-target[1])+
					abs(color[2]-target[2]);
	  }

  public:

	  // empty constructor
	  ColorDetector() : minDist(100) { 

		  // default parameter initialization here
		  target[0]= target[1]= target[2]= 0;
	  }

	  // Getters and setters

	  // Sets the color distance threshold.
	  // Threshold must be positive, otherwise distance threshold
	  // is set to 0.
	  void setColorDistanceThreshold(int distance) {

		  if (distance<0)
			  distance=0;
		  minDist= distance;
	  }

	  // Gets the color distance threshold
	  int getColorDistanceThreshold() const {

		  return minDist;
	  }

	  // Sets the color to be detected
	  void setTargetColor(unsigned char red, unsigned char green, unsigned char blue) {

		  target[2]= red;
		  target[1]= green;
		  target[0]= blue;
	  }

	  // Sets the color to be detected
	  void setTargetColor(cv::Vec3b color) {

		  target= color;
	  }

	  // Gets the color to be detected
	  cv::Vec3b getTargetColor() const {

		  return target;
	  }

	  // Processes the image. Returns a 1-channel binary image.
	  cv::Mat process(const cv::Mat &image);
};


#endif
