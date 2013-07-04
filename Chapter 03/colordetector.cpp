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

#include "colordetector.h"
	
cv::Mat ColorDetector::process(const cv::Mat &image) {
	
	  // re-allocate binary map if necessary
	  // same size as input image, but 1-channel
	  result.create(image.rows,image.cols,CV_8U);

	  // get the iterators
	  cv::Mat_<cv::Vec3b>::const_iterator it= image.begin<cv::Vec3b>();
	  cv::Mat_<cv::Vec3b>::const_iterator itend= image.end<cv::Vec3b>();
	  cv::Mat_<uchar>::iterator itout= result.begin<uchar>();

	  // for each pixel
	  for ( ; it!= itend; ++it, ++itout) {
        
		// process each pixel ---------------------

		  // compute distance from target color
		  if (getDistance(*it)<minDist) {

			  *itout= 255;

		  } else {

			  *itout= 0;
		  }

        // end of pixel processing ----------------
	  }

	  return result;
}

