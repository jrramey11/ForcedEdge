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
#include <fstream>
#include "edge.h"



const std::string filename = "/Users/jzeimen/current_semester/Scientific Vizualization/ForcedEdge/edges.txt";


std::vector<edge> get_edges(std::string file_name, double k, int n){
    std::ifstream file;
    file.open(file_name.c_str());
    std::vector<edge> edges;
    if(!file) {
        std::cout << std::endl << "Failed to open file " << filename;
        std::exit(1);
    }
    
    while(!file.eof()){
        double a_lat, b_lat, a_lon, b_lon;
        file >> a_lat;
        a_lat+=180;
        file >> a_lon;
        a_lon+=90;
        file >> b_lat;
        b_lat+=180;
        file>> b_lon;
        b_lon+=90;

        edge e(cv::Point2d(a_lat, a_lon), cv::Point2d(b_lat, b_lon), k, n);
        edges.push_back(e);
    }
    std::cout <<"Got here" << std::endl;
    
    
    return edges;
}


void draw_and_show_edges(std::vector<edge> edges){
    
    cv::Mat m = cv::Mat::ones(1000,1000,CV_8UC3);
    cv::rectangle(m, cv::Point(100,100), cv::Point(200,200), cv::Scalar( 0, 0, 255 ));
    
    cv::namedWindow("H");
    cv::imshow("H", m);
    
    cv::waitKey(0);
}


void bundle_iteration(std::vector<edge> edges){
    
}


int main(int argc, const char * argv[])
{
    get_edges(filename, .5, 4);
    
    
    
    return 0;
}

