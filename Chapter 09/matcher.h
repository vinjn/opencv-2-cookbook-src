/*------------------------------------------------------------------------------------------*\
   This file contains material supporting chapter 9 of the cookbook:  
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

#if !defined MATCHER
#define MATCHER

#include <vector>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/features2d/features2d.hpp>

class RobustMatcher {

  private:

	  // pointer to the feature point detector object
	  cv::Ptr<cv::FeatureDetector> detector;
	  // pointer to the feature descriptor extractor object
	  cv::Ptr<cv::DescriptorExtractor> extractor;
	  float ratio; // max ratio between 1st and 2nd NN
	  bool refineF; // if true will refine the F matrix
	  double distance; // min distance to epipolar
	  double confidence; // confidence level (probability)

  public:

	  RobustMatcher() : ratio(0.65f), refineF(true), confidence(0.99), distance(3.0) {	  

		  // SURF is the default feature
		  detector= new cv::SurfFeatureDetector();
		  extractor= new cv::SurfDescriptorExtractor();
	  }

	  // Set the feature detector
	  void setFeatureDetector(cv::Ptr<cv::FeatureDetector>& detect) {

		  detector= detect;
	  }

	  // Set descriptor extractor
	  void setDescriptorExtractor(cv::Ptr<cv::DescriptorExtractor>& desc) {

		  extractor= desc;
	  }

	  // Set the minimum distance to epipolar in RANSAC
	  void setMinDistanceToEpipolar(double d) {

		  distance= d;
	  }

	  // Set confidence level in RANSAC
	  void setConfidenceLevel(double c) {

		  confidence= c;
	  }

	  // Set the NN ratio
	  void setRatio(float r) {

		  ratio= r;
	  }

	  // if you want the F matrix to be recalculated
	  void refineFundamental(bool flag) {

		  refineF= flag;
	  }

	  // Clear matches for which NN ratio is > than threshold
	  // return the number of removed points 
	  // (corresponding entries being cleared, i.e. size will be 0)
	  int ratioTest(std::vector<std::vector<cv::DMatch>>& matches) {

		int removed=0;

        // for all matches
		for (std::vector<std::vector<cv::DMatch>>::iterator matchIterator= matches.begin();
			 matchIterator!= matches.end(); ++matchIterator) {

				 // if 2 NN has been identified
				 if (matchIterator->size() > 1) {

					 // check distance ratio
					 if ((*matchIterator)[0].distance/(*matchIterator)[1].distance > ratio) {

						 matchIterator->clear(); // remove match
						 removed++;
					 }

				 } else { // does not have 2 neighbours

					 matchIterator->clear(); // remove match
					 removed++;
				 }
		}

		return removed;
	  }

	  // Insert symmetrical matches in symMatches vector
	  void symmetryTest(const std::vector<std::vector<cv::DMatch>>& matches1,
		                const std::vector<std::vector<cv::DMatch>>& matches2,
					    std::vector<cv::DMatch>& symMatches) {
			
		// for all matches image 1 -> image 2
		for (std::vector<std::vector<cv::DMatch>>::const_iterator matchIterator1= matches1.begin();
			 matchIterator1!= matches1.end(); ++matchIterator1) {

			if (matchIterator1->size() < 2) // ignore deleted matches 
				continue;

			// for all matches image 2 -> image 1
			for (std::vector<std::vector<cv::DMatch>>::const_iterator matchIterator2= matches2.begin();
				matchIterator2!= matches2.end(); ++matchIterator2) {

				if (matchIterator2->size() < 2) // ignore deleted matches 
					continue;

				// Match symmetry test
				if ((*matchIterator1)[0].queryIdx == (*matchIterator2)[0].trainIdx  && 
					(*matchIterator2)[0].queryIdx == (*matchIterator1)[0].trainIdx) {

						// add symmetrical match
						symMatches.push_back(cv::DMatch((*matchIterator1)[0].queryIdx,
									  				    (*matchIterator1)[0].trainIdx,
													    (*matchIterator1)[0].distance));
						break; // next match in image 1 -> image 2
				}
			}
		}
	  }

	  // Identify good matches using RANSAC
	  // Return fundemental matrix
	  cv::Mat ransacTest(const std::vector<cv::DMatch>& matches,
		                 const std::vector<cv::KeyPoint>& keypoints1, 
						 const std::vector<cv::KeyPoint>& keypoints2,
					     std::vector<cv::DMatch>& outMatches) {

		// Convert keypoints into Point2f	
		std::vector<cv::Point2f> points1, points2;	
		for (std::vector<cv::DMatch>::const_iterator it= matches.begin();
			 it!= matches.end(); ++it) {

			 // Get the position of left keypoints
			 float x= keypoints1[it->queryIdx].pt.x;
			 float y= keypoints1[it->queryIdx].pt.y;
			 points1.push_back(cv::Point2f(x,y));
			 // Get the position of right keypoints
			 x= keypoints2[it->trainIdx].pt.x;
			 y= keypoints2[it->trainIdx].pt.y;
			 points2.push_back(cv::Point2f(x,y));
	    }

		// Compute F matrix using RANSAC
		std::vector<uchar> inliers(points1.size(),0);
		cv::Mat fundemental= cv::findFundamentalMat(
			cv::Mat(points1),cv::Mat(points2), // matching points
		    inliers,      // match status (inlier ou outlier)  
		    CV_FM_RANSAC, // RANSAC method
		    distance,     // distance to epipolar line
		    confidence);  // confidence probability
	
		// extract the surviving (inliers) matches
		std::vector<uchar>::const_iterator itIn= inliers.begin();
		std::vector<cv::DMatch>::const_iterator itM= matches.begin();
		// for all matches
		for ( ;itIn!= inliers.end(); ++itIn, ++itM) {

			if (*itIn) { // it is a valid match

				outMatches.push_back(*itM);
			}
		}

		std::cout << "Number of matched points (after cleaning): " << outMatches.size() << std::endl;

		if (refineF) {
		// The F matrix will be recomputed with all accepted matches

			// Convert keypoints into Point2f for final F computation	
			points1.clear();
			points2.clear();
	
			for (std::vector<cv::DMatch>::const_iterator it= outMatches.begin();
				 it!= outMatches.end(); ++it) {

				 // Get the position of left keypoints
				 float x= keypoints1[it->queryIdx].pt.x;
				 float y= keypoints1[it->queryIdx].pt.y;
				 points1.push_back(cv::Point2f(x,y));
				 // Get the position of right keypoints
				 x= keypoints2[it->trainIdx].pt.x;
				 y= keypoints2[it->trainIdx].pt.y;
				 points2.push_back(cv::Point2f(x,y));
			}

			// Compute 8-point F from all accepted matches
			fundemental= cv::findFundamentalMat(
				cv::Mat(points1),cv::Mat(points2), // matching points
				CV_FM_8POINT); // 8-point method
		}

		return fundemental;
	  }

	  // Match feature points using symmetry test and RANSAC
	  // returns fundemental matrix
	  cv::Mat match(cv::Mat& image1, cv::Mat& image2, // input images 
		  std::vector<cv::DMatch>& matches, // output matches and keypoints
		  std::vector<cv::KeyPoint>& keypoints1, std::vector<cv::KeyPoint>& keypoints2) {

		// 1a. Detection of the SURF features
		detector->detect(image1,keypoints1);
		detector->detect(image2,keypoints2);

		std::cout << "Number of SURF points (1): " << keypoints1.size() << std::endl;
		std::cout << "Number of SURF points (2): " << keypoints2.size() << std::endl;

		// 1b. Extraction of the SURF descriptors
		cv::Mat descriptors1, descriptors2;
		extractor->compute(image1,keypoints1,descriptors1);
		extractor->compute(image2,keypoints2,descriptors2);

		std::cout << "descriptor matrix size: " << descriptors1.rows << " by " << descriptors1.cols << std::endl;

		// 2. Match the two image descriptors

		// Construction of the matcher 
		cv::BruteForceMatcher<cv::L2<float>> matcher;

		// from image 1 to image 2
		// based on k nearest neighbours (with k=2)
		std::vector<std::vector<cv::DMatch>> matches1;
		matcher.knnMatch(descriptors1,descriptors2, 
			matches1, // vector of matches (up to 2 per entry) 
			2);		  // return 2 nearest neighbours

		// from image 2 to image 1
		// based on k nearest neighbours (with k=2)
		std::vector<std::vector<cv::DMatch>> matches2;
		matcher.knnMatch(descriptors2,descriptors1, 
			matches2, // vector of matches (up to 2 per entry) 
			2);		  // return 2 nearest neighbours

		std::cout << "Number of matched points 1->2: " << matches1.size() << std::endl;
		std::cout << "Number of matched points 2->1: " << matches2.size() << std::endl;

		// 3. Remove matches for which NN ratio is > than threshold

		// clean image 1 -> image 2 matches
		int removed= ratioTest(matches1);
		std::cout << "Number of matched points 1->2 (ratio test) : " << matches1.size()-removed << std::endl;
		// clean image 2 -> image 1 matches
		removed= ratioTest(matches2);
		std::cout << "Number of matched points 1->2 (ratio test) : " << matches2.size()-removed << std::endl;

		// 4. Remove non-symmetrical matches
	    std::vector<cv::DMatch> symMatches;
		symmetryTest(matches1,matches2,symMatches);

		std::cout << "Number of matched points (symmetry test): " << symMatches.size() << std::endl;

		// 5. Validate matches using RANSAC
		cv::Mat fundemental= ransacTest(symMatches, keypoints1, keypoints2, matches);

		// return the found fundemental matrix
		return fundemental;
	}
};

#endif
