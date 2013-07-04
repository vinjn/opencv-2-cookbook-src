/*------------------------------------------------------------------------------------------*\
   This file contains material supporting chapter 4 of the cookbook:  
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

#if !defined ICOMPARATOR
#define ICOMPARATOR

#include <opencv2\core\core.hpp>
#include <opencv2\imgproc\imgproc.hpp>
#include "colorhistogram.h"

class ImageComparator {

  private:

	cv::Mat reference;
	cv::Mat input;
	cv::MatND refH;
	cv::MatND inputH;

	ColorHistogram hist;
	int div;

  public:

	ImageComparator() : div(32) {

	}

	// Color reduction factor
	// The comparaison will be made on images with
	// color space reduced by this factor in each dimension
	void setColorReduction( int factor) {

		div= factor;
	}

	int getColorReduction() {

		return div;
	}

	void setReferenceImage(const cv::Mat& image) {

		reference= hist.colorReduce(image,div);
		refH= hist.getHistogram(reference);
	}

	double compare(const cv::Mat& image) {

		input= hist.colorReduce(image,div);
		inputH= hist.getHistogram(input);

		return cv::compareHist(refH,inputH,CV_COMP_INTERSECT);
	}
};


#endif
