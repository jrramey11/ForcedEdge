//
//  main.cpp
//  Force-Edge
//
//  Created by Jerry  Ramey on 4/28/13.
//  Copyright (c) 2013 Jerry  Ramey. All rights reserved.
//

#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>


int main(int argc, const char * argv[])
{

    // insert code here...
    
    cv::Mat m = cv::Mat::ones(1000,1000,CV_8UC3);
    cv::rectangle(m, cv::Point(100,100), cv::Point(200,200), cv::Scalar( 0, 0, 255 ));
    
    cv::namedWindow("H");
    cv::imshow("H", m);
    
    cv::waitKey(0);
    
    return 0;
}

