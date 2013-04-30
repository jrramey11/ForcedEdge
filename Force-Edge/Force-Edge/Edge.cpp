//
//  Edge.cpp
//  Force-Edge
//
//  Created by Joe Zeimen on 4/28/13.
//  Copyright (c) 2013 Jerry  Ramey. All rights reserved.
//

#include "Edge.h"
#include <cassert>

edge::edge(cv::Point2d start_point, cv::Point2d end_point, double k, int n ){
    
    start = start_point;
    end = end_point;
    K = k;
    
    assert(start.x >= 0);
    assert(start.y >= 0);
    assert(end.x >= 0);
    assert(end.y >= 0);

    
    //Create the initial intermediate points
    cv::Point2d diff = end-start;
    diff = cv::Point2d(diff.x/n, diff.y/n);
    cv::Point2d next =start + diff;
    for(int i=0; i<n-1; i++){
        points.push_back(next);
        assert(next.x >=0);
        assert(next.y>=0);
        next+=diff;
    }
    

    
    
    
}
cv::Point2d edge::operator() (int n){
    return points[n];
}

std::vector<cv::Point2d> edge::get_poly_line(){
    std::vector<cv::Point2d> ret_vec;
    ret_vec.push_back(start);
    ret_vec.insert(ret_vec.end(), points.begin(), points.end());
    ret_vec.push_back(end);
    for(std::vector<cv::Point2d>::iterator i = ret_vec.begin(); i!=ret_vec.end(); i++ ){
//        std::cout << *(i) << std::endl;
        assert(i->x >= 0);
        assert(i->y >= 0);
    }
    return ret_vec;
}

void edge::set(int n, cv::Point2d p){
    points[n]=p;
}