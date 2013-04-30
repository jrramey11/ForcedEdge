//
//  Edge.cpp
//  Force-Edge
//
//  Created by Joe Zeimen on 4/28/13.
//  Copyright (c) 2013 Jerry  Ramey. All rights reserved.
//

#include "Edge.h"


edge::edge(cv::Point2f start_point, cv::Point2f end_point, double k, int n ){
    
    start = start_point;
    end = end_point;
    K = k;
    
    std::cout<< start << " "<< end << std::endl;

    
    //Create the initial intermediate points
    cv::Point2f diff = end-start;
    diff = cv::Point2f(diff.x/n, diff.y/n);
    cv::Point2f next =start + diff;
    for(int i=0; i<n-1; i++){
        std::cout << next << std::endl;
        points.push_back(next);
        next+=diff;
    }
    
    
    
}
cv::Point2f edge::operator() (int n){
    return points[n];
}

std::vector<cv::Point2f> edge::get_poly_line(){
    std::vector<cv::Point2f> ret_vec;
    ret_vec.push_back(start);
    ret_vec.insert(ret_vec.end(), points.begin(), points.end());
    ret_vec.push_back(end);
    return ret_vec;
}

void edge::set(int n, cv::Point2f p){
    points[n]=p;
}