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

#include <iostream>
#include <opencv2/core.hpp>
#include <opencv2/features2d.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <vector>

int main()
{
    // Read input images
    cv::Mat image1 = cv::imread("../images/church01.jpg", 0);
    cv::Mat image2 = cv::imread("../images/church02.jpg", 0);
    if (!image1.data || !image2.data)
        return 0;

    // Display the images
    cv::namedWindow("Right Image");
    cv::imshow("Right Image", image1);
    cv::namedWindow("Left Image");
    cv::imshow("Left Image", image2);

    // vector of keypoints
    std::vector<cv::KeyPoint> keypoints1;
    std::vector<cv::KeyPoint> keypoints2;

    // Construction of the SURF feature detector
    auto surf = cv::ORB::create();

    // Detection of the SURF features
    surf->detect(image1, keypoints1);
    surf->detect(image2, keypoints2);

    std::cout << "Number of SURF points (1): " << keypoints1.size() << std::endl;
    std::cout << "Number of SURF points (2): " << keypoints2.size() << std::endl;

    // Draw the kepoints
    cv::Mat imageKP;
    cv::drawKeypoints(image1, keypoints1, imageKP, cv::Scalar(255, 255, 255),
                      cv::DrawMatchesFlags::DRAW_RICH_KEYPOINTS);
    cv::namedWindow("Right SURF Features");
    cv::imshow("Right SURF Features", imageKP);
    cv::drawKeypoints(image2, keypoints2, imageKP, cv::Scalar(255, 255, 255),
                      cv::DrawMatchesFlags::DRAW_RICH_KEYPOINTS);
    cv::namedWindow("Left SURF Features");
    cv::imshow("Left SURF Features", imageKP);

    // Extraction of the SURF descriptors
    cv::Mat descriptors1, descriptors2;
    surf->compute(image1, keypoints1, descriptors1);
    surf->compute(image2, keypoints2, descriptors2);

    std::cout << "descriptor matrix size: " << descriptors1.rows << " by " << descriptors1.cols
              << std::endl;

    // Construction of the matcher
    auto matcher = cv::BFMatcher::create();

    // Match the two image descriptors
    std::vector<cv::DMatch> matches;
    matcher->match(descriptors1, descriptors2, matches);

    std::cout << "Number of matched points: " << matches.size() << std::endl;

    std::nth_element(matches.begin(),      // initial position
                     matches.begin() + 24, // position of the sorted element
                     matches.end());       // end position
    // remove all elements after the 25th
    matches.erase(matches.begin() + 25, matches.end());

    cv::Mat imageMatches;
    cv::drawMatches(image1, keypoints1,         // 1st image and its keypoints
                    image2, keypoints2,         // 2nd image and its keypoints
                    matches,                    // the matches
                    imageMatches,               // the image produced
                    cv::Scalar(255, 255, 255)); // color of the lines
    cv::namedWindow("Matches");
    cv::imshow("Matches", imageMatches);

    cv::waitKey();
    return 0;

    int size = 7;
    cv::Mat imaf1;
    image1.convertTo(imaf1, CV_32F);

    cv::Mat imaf2;
    image2.convertTo(imaf2, CV_32F);

    cv::waitKey();
    return 0;
}