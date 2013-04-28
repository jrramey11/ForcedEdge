//
//  Edge.h
//  Force-Edge
//
//  Created by Joe Zeimen on 4/28/13.
//  Copyright (c) 2013 Jerry  Ramey. All rights reserved.
//

#ifndef __Force_Edge__Edge__
#define __Force_Edge__Edge__

#include <iostream>
#include <opencv2/opencv.hpp>

class edge {
private:
    double K;
    cv::Point2f start;
    cv::Point2f end;
    std::vector<cv::Point2f> points;
public:
    edge(cv::Point2f, cv::Point2f, double K, int N);
    
};


#endif /* defined(__Force_Edge__Edge__) */
