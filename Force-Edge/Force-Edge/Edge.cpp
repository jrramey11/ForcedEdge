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
    

    
    cv::Point2f diff = start-end;
    //diff = diff/cv::Point2f(2,2);
    
    
    
    
}