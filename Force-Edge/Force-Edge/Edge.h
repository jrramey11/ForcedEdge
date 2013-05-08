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
#include <opencv/cv.h>

class edge {
private:
    double Kp;
    cv::Point2d start;
    cv::Point2d end;
    std::vector<cv::Point2d> points;
public:
    edge(cv::Point2d, cv::Point2d, double K, int N);
    std::vector<cv::Point2d> get_poly_line();
    cv::Point2d operator()(int n);
    cv::Point2d get_force_at(int n);
    void set(int , cv::Point2d);
    cv::Point2d get_start(){return start;};
    cv::Point2d get_end(){return end;};
};


#endif /* defined(__Force_Edge__Edge__) */
