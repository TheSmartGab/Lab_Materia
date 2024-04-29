#ifndef General_h
#define General_h

#include <iostream>
#include <vector>
#include <fstream>
#include <cmath>
#include <math.h>
#include <filesystem>

#include "TF1.h"
#include "TGraph.h"
#include "TCanvas.h"
#include "TAxis.h"
#include "TLegend.h"
#include "TMultiGraph.h"
#include "TMath.h"

using namespace std;

class Measure;
class Measurements;
class TabValues;
class Run;
class Run_Info;
class Glass_Info;
class Disk_Geo;

extern int debug;
extern int print;
extern int fit;
extern string path;
extern string name_print;

static string BIAS_FILE = "../Lab_Data/20_03_24/200324_aria_aria_1.txt";
static string Run_Data_Dir = "../Run_Data/";
static string Disk_Geo_File = "../Run_Info/General/Disk_Geo.txt";

static double XL_DELTA = 0.25;
static double YL_DELTA = 0.05;
static double delta = 0.000001; //to avoid double == double logic other tan double == 0

static double Legend_Text_size = 0.04;

inline void ResetLegend(double& xl1, double& xl2, double& yl1, double& yl2){
    xl1 = 0;
    xl2 = xl1 + XL_DELTA;
    yl1 = 1-YL_DELTA;
    yl2 = 1;
}

//X right edge
inline bool XREdge(const double& xl1, const double& yl1){ //returns true if the current legend position results in the legend touching the right edge
    return (xl1 > 1 - 2*XL_DELTA) ? true : false ;
}
//X left edge
inline bool XLEdge(const double& xl1, const double& yl1){ //I want the legends to fill the upper and righter part of the canvases
    if(yl1 > 1 - 2 * YL_DELTA - delta){ //manage the 2 upper layer, note that double == double is not reliable
        return (xl1 == 0) ? true : false;
    }
    else{
        return (xl1 < 1 - XL_DELTA + delta) ? true : false ;
    }
}
//Y upper edge
inline bool YUEdge(const double& xl1, const double& yl1){
    return (yl1 > 1 - YL_DELTA - delta) ? true : false; 
}
//Y lower edge
inline bool YLEdge(const double& xl1, const double& yl1){
    if(xl1 > 1 - 2*XL_DELTA){
        return (yl1 == 0) ? true : false;
    }
    else{
        return (yl1 < 1 - YL_DELTA + delta) ? true : false;
    }
}

inline void UpdateLegendLimits(double& xl1, double& xl2, double& yl1, double& yl2){
    if(!XREdge(xl1, yl1)){
        xl1 += XL_DELTA;
        xl2 += XL_DELTA;
        return ;
    }
    else if(!YLEdge(xl1, xl2)){
        yl1 -= YL_DELTA;
        yl2 -= YL_DELTA;
        while(!XLEdge(xl1, yl1)){
            xl1 -= XL_DELTA;
            xl2 -= XL_DELTA;
        }
        return ;
    }
    cout<<"Warning: Need more space for legend, legends are beeing overlayered"<<endl;
}


#endif 