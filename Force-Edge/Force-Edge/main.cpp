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
const int SCALE_FACTOR = 4;

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
        file >> a_lon;
        file >> a_lat;
        file >> b_lon;
        file >> b_lat;
        a_lat =(a_lat+90)*SCALE_FACTOR;
        a_lon =(180-a_lon+90)*SCALE_FACTOR;
        b_lat =(b_lat+90)*SCALE_FACTOR;
        b_lon =(180-b_lon+90)*SCALE_FACTOR;

        edge e(cv::Point2d(a_lat, a_lon), cv::Point2d(b_lat, b_lon), k, n);
        edges.push_back(e);
    }
    std::cout <<"Got here" << std::endl;
    
    
    return edges;
}

std::vector<std::vector<cv::Point> > d_to_i(std::vector<std::vector<cv::Point2d> > pts ){
    std::vector<std::vector<cv::Point> > ret_vec;
    for(std::vector<std::vector<cv::Point2d> >::iterator i = pts.begin(); i!= pts.end(); i++){
        std::vector<cv::Point> point_list;
        for(std::vector<cv::Point2d>::iterator j = i->begin(); j!=i->end(); j++){
            point_list.push_back(cv::Point(j->x+0.5,j->y+0.5));
        }
        ret_vec.push_back(point_list);
    }
    
    return ret_vec;
    
}

void draw_and_show_edges(std::vector<edge> edges){
    
    cv::Mat accumulation = cv::Mat::zeros(180*SCALE_FACTOR,360*SCALE_FACTOR,CV_16UC1);

    
    std::vector<std::vector<cv::Point2d> > lines;
    int count =0;
    for(std::vector<edge>::iterator i = edges.begin(); i!=edges.end(); i++){
        cv::Mat m = cv::Mat::zeros(180*SCALE_FACTOR,360*SCALE_FACTOR,CV_16UC1);
        lines.push_back(i->get_poly_line());
        cv::polylines(m, d_to_i(lines), false, cv::Scalar(0x00FF), 1, CV_AA);
        accumulation+=m;
        lines.clear();
        if(!(count++%300)) std::cout << (double)count*100/ edges.size()<< std::endl;
    }
    
    
    cv::imwrite("/Users/jzeimen/Desktop/img.png", accumulation);
    cv::namedWindow("H");
    cv::imshow("H", accumulation);
    cv::waitKey(0);
}


void bundle_iteration(std::vector<edge> edges){
    
}


int main(int argc, const char * argv[])
{
    
    std::vector<edge> edges = get_edges(filename, .5, 4);
    draw_and_show_edges(edges);
    
    
    return 0;
}

