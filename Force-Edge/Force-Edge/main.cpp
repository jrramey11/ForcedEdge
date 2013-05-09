//
//  main.cpp
//  Force-Edge
//
//  Created by Jerry  Ramey on 4/28/13.
//  Copyright (c) 2013 Jerry  Ramey. All rights reserved.
//

#include <iostream>
#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <fstream>
#include <iomanip>      // std::setprecision
#include "Edge.h"
#include <sstream>
#include <omp.h>


//const std::string filename = "/Users/jzeimen/current_semester/Scientific Vizualization/ForcedEdge/edges.txt";
//const std::string filename = "/home/jzeimen/Documents/school/College/Spring2013/Scientific Vizualization/ForcedEdge/edges.txt";
const std::string filename = "/Users/jrramey11/Desktop/ForcedEdge/edges.txt";
const int SCALE_FACTOR = 32;


double visibility_half(cv::Point2d q1, cv::Point2d q2, cv::Point2d p1, cv::Point2d p2 ){
    double L_m = -((q1.x-q2.x)/(q1.y-q2.y));
    if(0==q1.y-q2.y) L_m=0.0;
    double L_1b = q1.y-L_m*q1.x;
    double L_2b = q2.y-L_m*q2.x;
    
    double Mpx = (p1.x+p2.x)/2;
    double Mpy = (p1.y+p2.y)/2;
    
    double P_m = (p1.y-p2.y)/(p1.x-p2.x);
    double P_b = p1.y-P_m*p1.x;
    
    double i1_x = (P_b-L_1b)/(L_m-P_m);
    double i1_y = L_m*i1_x+L_1b;
    
    double i2_x = (P_b-L_2b)/(L_m-P_m);
    double i2_y = L_m*i2_x+L_2b;
    
    double I_m_x = (i1_x+i2_x)/2;
    double I_m_y = (i1_y+i2_y)/2;
    
    double M_dx = Mpx-I_m_x;
    double M_dy = Mpy-I_m_y;
    
    
    double I_dx = i1_x-i2_x;
    double I_dy = i1_y-i2_y;
    
    double Pm_Im_dist = std::sqrt(M_dx*M_dx+M_dy*M_dy);
    double I_length = std::sqrt(I_dx*I_dx+I_dy*I_dy);
    double ret = std::max((1-(2*Pm_Im_dist/I_length)),0.0);
    return ret;
}


double visibility(cv::Point2d q1, cv::Point2d q2, cv::Point2d p1, cv::Point2d p2 ){
    return std::min(visibility_half(q1,q2,p1,p2),visibility_half(p1,p2,q1,q2));
}



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
        a_lat =(a_lat+90);
        a_lon =(180-a_lon+90);
        b_lat =(b_lat+90);
        b_lon =(180-b_lon+90);
        
        //US
        //Upper left corner and lower right corner of the ROI
        double ULX = 52;
        double ULY = 106;
        double LRX = 33;
        double LRY = 65;
        if(a_lat < ULX || a_lat > ULY || b_lat < ULX || b_lat > ULY ||
           a_lon < LRX || a_lon > LRY || b_lon < LRX || b_lon > LRY) continue;
        
        
        edge e(cv::Point2d(a_lat*SCALE_FACTOR, a_lon*SCALE_FACTOR), cv::Point2d(b_lat*SCALE_FACTOR, b_lon*SCALE_FACTOR), k, n);
        edges.push_back(e);
    }
    
    
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

void draw_and_show_edges(std::vector<edge> edges, std::string file_name){
    
    cv::Mat accumulation = cv::Mat::zeros(180*SCALE_FACTOR/2,360*SCALE_FACTOR/2,CV_16UC1);
    
    
    std::vector<std::vector<cv::Point2d> > lines;
    int count =0;
    for(std::vector<edge>::iterator i = edges.begin(); i!=edges.end(); i++){
        cv::Mat m = cv::Mat::zeros(180*SCALE_FACTOR/2,360*SCALE_FACTOR/2,CV_16UC1);
        lines.push_back(i->get_poly_line());
        cv::polylines(m, d_to_i(lines), false, cv::Scalar(0x02FF), 1, CV_AA);
        accumulation+=m;
        lines.clear();
        if(!(count++%500)) std::cout << (double)count*100/ edges.size()<< "%" << std::endl;
    }
    
    
    cv::imwrite(file_name, accumulation);
    //    cv::namedWindow("H");
    //    cv::imshow("H", accumulation);
    //    cv::waitKey(0);
}

double compatability(cv::Point2d start_a, cv::Point2d end_a, cv::Point2d start_b, cv::Point2d end_b ){
    //Common numbers
    const cv::Point2d P = start_a-end_a;
    const cv::Point2d Q = start_b-end_b;
    const double P_length = cv::norm(P);
    const double Q_length = cv::norm(Q);
    
    //Angle similarity metric
    //a is 1 when parallel
    //a is 0 when perpendicular
    double P_dot_Q_over_PQ = (P.x*Q.x+P.y*Q.y)/(P_length*Q_length);
    double a = std::abs(P_dot_Q_over_PQ);
    
    //Scale differences
    //s = 0 when very different in scale
    //s = 1 when they are close in scale
    double l_ave = (P_length+Q_length)/2;
    double s = 2/(l_ave*std::min(P_length,Q_length)+  std::max(P_length,Q_length)/l_ave);
    
    //Edges that are far apart
    const cv::Point2d P_m((start_a.x+end_a.x)/2,(start_a.y+end_a.y)/2);
    const cv::Point2d Q_m((start_b.x+end_b.x)/2,(start_b.y+end_a.y)/2);
    double p = l_ave/(l_ave+(cv::norm(P_m-Q_m)));
    
    double v = visibility(start_a, end_a, start_b, end_b);
    
    
    assert(!std::isnan(a));
    assert(!std::isnan(s));
    assert(!std::isnan(p));
    assert(!std::isnan(v));

    return 10000*a*s*p*v;
}


cv::Mat_<double> create_compatabilities(std::vector<edge> edges){
    cv::Mat_<double> ret_mat(edges.size(),edges.size(),0.0);
#pragma omp parallel for
    for(int i = 0; i<edges.size(); i++){
        for(int j=0; j<edges.size(); j++){
            cv::Point2d start_a, end_a, start_b, end_b;
            start_a = edges[i].get_start();
            end_a = edges[i].get_end();
            start_b = edges[j].get_start();
            end_b = edges[j].get_end();
            compatability(start_a,end_a,start_b,end_b);
            ret_mat(i,j)=compatability(start_a,end_a,start_b,end_b);
        }
    }
    return ret_mat;
    
};




void bundle_iteration(std::vector<edge> &edges, std::vector<edge> &copy, int n, cv::Mat_<double> &comps){
    
    
    
    //For every edge
#pragma omp parallel for
    for(int i =0; i<edges.size(); i++){
        //For every internal point in the edge
        for(int j = 0; j<n-1; j++){
            cv::Point2d spring_force = edges[i].get_force_at(j);
            cv::Point2d electro_static_force(0,0);
            //Sum the forces acted on by all other edges
            for(int k=0; k<edges.size(); k++){
                if(k==i) continue;
                cv::Point2d vec = (edges[k](j)-edges[i](j));
                double dist = std::sqrt(vec.x*vec.x+vec.y*vec.y);
                if(dist != 0){
                    double f = 1/dist;
                    vec.x = f*vec.x/dist;
                    vec.y = f*vec.y/dist;
                }
                double x = comps(i,k);
                vec.x = x* vec.x;
                vec.y = x*vec.y;
                electro_static_force += vec;
            }
            cv::Point2d force = spring_force + electro_static_force;
            double force_length = cv::norm(force);
            if (force_length ==0) continue;
            force.x = force.x/force_length;
            force.y = force.y/force_length;
            //Move it this many px
            force *= SCALE_FACTOR/8.0;
            cv::Point2d new_pos = edges[i](j)+force;
            copy[i].set(j, new_pos);
        }
    }
    
}


int main(int argc, const char * argv[])
{




    for(double i = 2; i<100; i = i+5){

        std::cout << "Reading file " <<std::endl;
        std::vector<edge> edges = get_edges(filename, i, 10);
        std::vector<edge> copy = get_edges(filename, i, 10);
        std::cout << edges.size() << std::endl;
        std::cout <<"creating compatabilities" << std::endl;
        cv::Mat_<double> m = create_compatabilities(edges);
        std::cout << "Core algorithm " <<std::endl;
        for(int j =0; j<40; j++){
            bundle_iteration(edges, copy, 10, m);
            bundle_iteration(copy, edges, 10, m);
            std::cout << j << std::endl;

        }
        
        std::cout << "Draw results: ";
        
        std::stringstream file_name;

        file_name << "/Users/jrramey11/Desktop/ForcedEdge/" << i << ".png";
        std::cout << file_name.str() << std::endl;
        draw_and_show_edges(edges,file_name.str());
    }
    
    for(double i = 125; i<1000; i = i+25){
        std::cout << "Reading file " <<std::endl;
        std::vector<edge> edges = get_edges(filename, i, 10);
        std::vector<edge> copy = get_edges(filename, i, 10);

        std::cout <<"creating compatabilities" << std::endl;
        cv::Mat_<double> m = create_compatabilities(edges);
        std::cout << "Core algorithm " <<std::endl;
        for(int j =0; j<40; j++){
            bundle_iteration(edges, copy, 10, m);
            bundle_iteration(copy, edges, 10, m);
            std::cout << j << std::endl;
            
        }
        
        std::cout << "Draw results: ";
        
        std::stringstream file_name;
        
        file_name << "/Users/jrramey11/Desktop/ForcedEdge/" << i << ".png";
        std::cout << file_name.str() << std::endl;
        draw_and_show_edges(edges,file_name.str());
    }
    
    return 0;
}

