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

// using .ptr and []
void colorReduce0(cv::Mat &image, int div=64) {

	  int nl= image.rows; // number of lines
	  int nc= image.cols * image.channels(); // total number of elements per line
              
      for (int j=0; j<nl; j++) {

		  uchar* data= image.ptr<uchar>(j);

          for (int i=0; i<nc; i++) {
 
            // process each pixel ---------------------
                 
                  data[i]= data[i]/div*div + div/2;
 
            // end of pixel processing ----------------
 
            } // end of line                   
      }
}

// using .ptr and * ++ 
void colorReduce1(cv::Mat &image, int div=64) {

	  int nl= image.rows; // number of lines
	  int nc= image.cols * image.channels(); // total number of elements per line
              
      for (int j=0; j<nl; j++) {

		  uchar* data= image.ptr<uchar>(j);

          for (int i=0; i<nc; i++) {
 
            // process each pixel ---------------------
                 
				 *data++= *data/div*div + div/2;
 
            // end of pixel processing ----------------
 
            } // end of line                   
      }
}

// using .ptr and * ++ and modulo
void colorReduce2(cv::Mat &image, int div=64) {

	  int nl= image.rows; // number of lines
	  int nc= image.cols * image.channels(); // total number of elements per line
              
      for (int j=0; j<nl; j++) {

		  uchar* data= image.ptr<uchar>(j);

          for (int i=0; i<nc; i++) {
 
            // process each pixel ---------------------
       
			      int v= *data;
                  *data++= v - v%div + div/2;
 
            // end of pixel processing ----------------
 
            } // end of line                   
      }
}

// using .ptr and * ++ and bitwise
void colorReduce3(cv::Mat &image, int div=64) {

	  int nl= image.rows; // number of lines
	  int nc= image.cols * image.channels(); // total number of elements per line
	  int n= static_cast<int>(log(static_cast<double>(div))/log(2.0));
	  // mask used to round the pixel value
	  uchar mask= 0xFF<<n; // e.g. for div=16, mask= 0xF0
              
      for (int j=0; j<nl; j++) {

		  uchar* data= image.ptr<uchar>(j);

          for (int i=0; i<nc; i++) {
 
            // process each pixel ---------------------
                 
            *data++= *data&mask + div/2;
 
            // end of pixel processing ----------------
 
            } // end of line                   
      }
}


// direct pointer arithmetic
void colorReduce4(cv::Mat &image, int div=64) {

	  int nl= image.rows; // number of lines
	  int nc= image.cols * image.channels(); // total number of elements per line
	  int n= static_cast<int>(log(static_cast<double>(div))/log(2.0));
	  int step= image.step; // effective width
	  // mask used to round the pixel value
	  uchar mask= 0xFF<<n; // e.g. for div=16, mask= 0xF0
              
      // get the pointer to the image buffer
	  uchar *data= image.data;

      for (int j=0; j<nl; j++) {

          for (int i=0; i<nc; i++) {
 
            // process each pixel ---------------------
                 
            *(data+i)= *data&mask + div/2;
 
            // end of pixel processing ----------------
 
            } // end of line                   

            data+= step;  // next line
      }
}

// using .ptr and * ++ and bitwise with image.cols * image.channels()
void colorReduce5(cv::Mat &image, int div=64) {

	  int nl= image.rows; // number of lines
	  int n= static_cast<int>(log(static_cast<double>(div))/log(2.0));
	  // mask used to round the pixel value
	  uchar mask= 0xFF<<n; // e.g. for div=16, mask= 0xF0
              
      for (int j=0; j<nl; j++) {

		  uchar* data= image.ptr<uchar>(j);

          for (int i=0; i<image.cols * image.channels(); i++) {
 
            // process each pixel ---------------------
                 
            *data++= *data&mask + div/2;
 
            // end of pixel processing ----------------
 
            } // end of line                   
      }
}

// using .ptr and * ++ and bitwise (continuous)
void colorReduce6(cv::Mat &image, int div=64) {

	  int nl= image.rows; // number of lines
	  int nc= image.cols * image.channels(); // total number of elements per line

	  if (image.isContinuous())  {
		  // then no padded pixels
		  nc= nc*nl; 
		  nl= 1;  // it is now a 1D array
	   }

	  int n= static_cast<int>(log(static_cast<double>(div))/log(2.0));
	  // mask used to round the pixel value
	  uchar mask= 0xFF<<n; // e.g. for div=16, mask= 0xF0
              
      for (int j=0; j<nl; j++) {

		  uchar* data= image.ptr<uchar>(j);

          for (int i=0; i<nc; i++) {
 
            // process each pixel ---------------------
                 
            *data++= *data&mask + div/2;
 
            // end of pixel processing ----------------
 
            } // end of line                   
      }
}

// using .ptr and * ++ and bitwise (continuous+channels)
void colorReduce7(cv::Mat &image, int div=64) {

	  int nl= image.rows; // number of lines
	  int nc= image.cols ; // number of columns

	  if (image.isContinuous())  {
		  // then no padded pixels
		  nc= nc*nl; 
		  nl= 1;  // it is now a 1D array
	   }

	  int n= static_cast<int>(log(static_cast<double>(div))/log(2.0));
	  // mask used to round the pixel value
	  uchar mask= 0xFF<<n; // e.g. for div=16, mask= 0xF0
              
      for (int j=0; j<nl; j++) {

		  uchar* data= image.ptr<uchar>(j);

          for (int i=0; i<nc; i++) {
 
            // process each pixel ---------------------
                 
            *data++= *data&mask + div/2;
            *data++= *data&mask + div/2;
            *data++= *data&mask + div/2;
 
            // end of pixel processing ----------------
 
            } // end of line                   
      }
}


// using Mat_ iterator 
void colorReduce8(cv::Mat &image, int div=64) {

	  // get iterators
	  cv::Mat_<cv::Vec3b>::iterator it= image.begin<cv::Vec3b>();
	  cv::Mat_<cv::Vec3b>::iterator itend= image.end<cv::Vec3b>();

	  for ( ; it!= itend; ++it) {
        
		// process each pixel ---------------------

        (*it)[0]= (*it)[0]/div*div + div/2;
        (*it)[1]= (*it)[1]/div*div + div/2;
        (*it)[2]= (*it)[2]/div*div + div/2;

        // end of pixel processing ----------------
	  }
}

// using Mat_ iterator and bitwise
void colorReduce9(cv::Mat &image, int div=64) {

	  // div must be a power of 2
	  int n= static_cast<int>(log(static_cast<double>(div))/log(2.0));
	  // mask used to round the pixel value
	  uchar mask= 0xFF<<n; // e.g. for div=16, mask= 0xF0

	  // get iterators
	  cv::Mat_<cv::Vec3b>::iterator it= image.begin<cv::Vec3b>();
	  cv::Mat_<cv::Vec3b>::iterator itend= image.end<cv::Vec3b>();

	  // scan all pixels
	  for ( ; it!= itend; ++it) {
        
		// process each pixel ---------------------

        (*it)[0]= (*it)[0]&mask + div/2;
        (*it)[1]= (*it)[1]&mask + div/2;
        (*it)[2]= (*it)[2]&mask + div/2;

        // end of pixel processing ----------------
	  }
}

// using MatIterator_ 
void colorReduce10(cv::Mat &image, int div=64) {

	  // get iterators
	  cv::Mat_<cv::Vec3b> cimage= image;
	  cv::Mat_<cv::Vec3b>::iterator it=cimage.begin();
	  cv::Mat_<cv::Vec3b>::iterator itend=cimage.end();

	  for ( ; it!= itend; it++) { 
        
		// process each pixel ---------------------

        (*it)[0]= (*it)[0]/div*div + div/2;
        (*it)[1]= (*it)[1]/div*div + div/2;
        (*it)[2]= (*it)[2]/div*div + div/2;

        // end of pixel processing ----------------
	  }
}


void colorReduce11(cv::Mat &image, int div=64) {

	  int nl= image.rows; // number of lines
	  int nc= image.cols; // number of columns
              
      for (int j=0; j<nl; j++) {
          for (int i=0; i<nc; i++) {
 
            // process each pixel ---------------------
                 
                  image.at<cv::Vec3b>(j,i)[0]=	 image.at<cv::Vec3b>(j,i)[0]/div*div + div/2;
                  image.at<cv::Vec3b>(j,i)[1]=	 image.at<cv::Vec3b>(j,i)[1]/div*div + div/2;
                  image.at<cv::Vec3b>(j,i)[2]=	 image.at<cv::Vec3b>(j,i)[2]/div*div + div/2;
 
            // end of pixel processing ----------------
 
            } // end of line                   
      }
}

// with input/ouput images
void colorReduce12(const cv::Mat &image, // input image 
                 cv::Mat &result,      // output image
                 int div=64) {

	  int nl= image.rows; // number of lines
	  int nc= image.cols ; // number of columns

	  // allocate output image if necessary
	  result.create(image.rows,image.cols,image.type());

	  // created images have no padded pixels
	  nc= nc*nl; 
	  nl= 1;  // it is now a 1D array

	  int n= static_cast<int>(log(static_cast<double>(div))/log(2.0));
	  // mask used to round the pixel value
	  uchar mask= 0xFF<<n; // e.g. for div=16, mask= 0xF0
              
      for (int j=0; j<nl; j++) {

		  uchar* data= result.ptr<uchar>(j);
		  const uchar* idata= image.ptr<uchar>(j);

          for (int i=0; i<nc; i++) {
 
            // process each pixel ---------------------
                 
            *data++= (*idata++)&mask + div/2;
            *data++= (*idata++)&mask + div/2;
            *data++= (*idata++)&mask + div/2;
 
            // end of pixel processing ----------------
 
          } // end of line                   
      }
}

// using overloaded operators
void colorReduce13(cv::Mat &image, int div=64) {
	
	  int n= static_cast<int>(log(static_cast<double>(div))/log(2.0));
	  // mask used to round the pixel value
	  uchar mask= 0xFF<<n; // e.g. for div=16, mask= 0xF0

	  // perform color reduction
	  image=(image&cv::Scalar(mask,mask,mask))+cv::Scalar(div/2,div/2,div/2);
}


#define NTESTS 14
#define NITERATIONS 20

int main()
{
	int64 t[NTESTS],tinit;
	cv::Mat image1;
	cv::Mat image2;

	// timer values set to 0
	for (int i=0; i<NTESTS; i++)
		t[i]= 0;

	// repeat the tests several times
	int n=NITERATIONS;
	for (int k=0; k<n; k++) {

		std::cout << k << " of " << n << std::endl; 

		image1= cv::imread("../image.jpg");
	    if (!image1.data)
		   return 0; 

		// using .ptr and []
	    tinit= cv::getTickCount();
		colorReduce0(image1);
		t[0]+= cv::getTickCount()-tinit;

		image1= cv::imread("../image.jpg");
		// using .ptr and * ++ 
	    tinit= cv::getTickCount();
		colorReduce1(image1);
		t[1]+= cv::getTickCount()-tinit;

		image1= cv::imread("../image.jpg");
		// using .ptr and * ++ and modulo
	    tinit= cv::getTickCount();
		colorReduce2(image1);
		t[2]+= cv::getTickCount()-tinit;

		image1= cv::imread("../image.jpg");
		// using .ptr and * ++ and bitwise
	    tinit= cv::getTickCount();
		colorReduce3(image1);
		t[3]+= cv::getTickCount()-tinit;

		image1= cv::imread("../image.jpg");
		// using direct pointer arithmetic
	    tinit= cv::getTickCount();
		colorReduce4(image1);
		t[4]+= cv::getTickCount()-tinit;

		image1= cv::imread("../image.jpg");
		// using .ptr and * ++ and bitwise with image.cols * image.channels()
	    tinit= cv::getTickCount();
		colorReduce5(image1);
		t[5]+= cv::getTickCount()-tinit;

		image1= cv::imread("../image.jpg");
		// using .ptr and * ++ and bitwise (continuous)
	    tinit= cv::getTickCount();
		colorReduce6(image1);
		t[6]+= cv::getTickCount()-tinit;

		image1= cv::imread("../image.jpg");
		// using .ptr and * ++ and bitwise (continuous+channels)
	    tinit= cv::getTickCount();
		colorReduce7(image1);
		t[7]+= cv::getTickCount()-tinit;

		image1= cv::imread("../image.jpg");
		// using Mat_ iterator
	    tinit= cv::getTickCount();
		colorReduce8(image1);
		t[8]+= cv::getTickCount()-tinit;

		image1= cv::imread("../image.jpg");
		// using Mat_ iterator and bitwise
	    tinit= cv::getTickCount();
		colorReduce9(image1);
		t[9]+= cv::getTickCount()-tinit;

		image1= cv::imread("../image.jpg");
		// using Mat_ iterator 
	    tinit= cv::getTickCount();
		colorReduce10(image1);
		t[10]+= cv::getTickCount()-tinit;

		image1= cv::imread("../image.jpg");
		// using at 
	    tinit= cv::getTickCount();
		colorReduce11(image1);
		t[11]+= cv::getTickCount()-tinit;
		
		image1= cv::imread("../image.jpg");
		// using input/output images 
	    tinit= cv::getTickCount();
		cv::Mat result;
		colorReduce12(image1, result);
		t[12]+= cv::getTickCount()-tinit;

		image2= result;
		
		image1= cv::imread("../image.jpg");
		// using input/output images 
	    tinit= cv::getTickCount();
		colorReduce13(image1);
		t[13]+= cv::getTickCount()-tinit;

		//------------------------------
	}
	    
	cv::namedWindow("Result");
	cv::imshow("Result",image2);
	cv::namedWindow("Image Result");
	cv::imshow("Image Result",image1);

	// print average execution time
	std::cout << std::endl << "-------------------------------------------" << std::endl << std::endl;
	std::cout << "using .ptr and [] =" << 1000.*t[0]/cv::getTickFrequency()/n << "ms" << std::endl;
	std::cout << "using .ptr and * ++ =" << 1000.*t[1]/cv::getTickFrequency()/n << "ms" << std::endl;
	std::cout << "using .ptr and * ++ and modulo =" << 1000.*t[2]/cv::getTickFrequency()/n << "ms" << std::endl;
	std::cout << "using .ptr and * ++ and bitwise =" << 1000.*t[3]/cv::getTickFrequency()/n << "ms" << std::endl;
	std::cout << "using direct pointer arithmetic =" << 1000.*t[4]/cv::getTickFrequency()/n << "ms" << std::endl;
	std::cout << "using .ptr and * ++ and bitwise with image.cols * image.channels() =" << 1000.*t[5]/cv::getTickFrequency()/n << "ms" << std::endl;
	std::cout << "using .ptr and * ++ and bitwise (continuous) =" << 1000.*t[6]/cv::getTickFrequency()/n << "ms" << std::endl;
	std::cout << "using .ptr and * ++ and bitwise (continuous+channels) =" << 1000.*t[7]/cv::getTickFrequency()/n << "ms" << std::endl;
	std::cout << "using Mat_ iterator =" << 1000.*t[8]/cv::getTickFrequency()/n << "ms" << std::endl;
	std::cout << "using Mat_ iterator and bitwise =" << 1000.*t[9]/cv::getTickFrequency()/n << "ms" << std::endl;
	std::cout << "using MatIterator_ =" << 1000.*t[10]/cv::getTickFrequency()/n << "ms" << std::endl;	
	std::cout << "using at =" << 1000.*t[11]/cv::getTickFrequency()/n << "ms" << std::endl;	
	std::cout << "using input/output images =" << 1000.*t[12]/cv::getTickFrequency()/n << "ms" << std::endl;	
	std::cout << "using overloaded operators =" << 1000.*t[13]/cv::getTickFrequency()/n << "ms" << std::endl;	
	
	cv::waitKey();
	return 0;
}
