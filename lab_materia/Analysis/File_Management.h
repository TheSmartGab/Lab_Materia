#ifndef __File_Management_h__
#define __File_Management_h__

#include <sstream>
#include <iostream>
#include <fstream>
#include <cmath>
#include <vector>
#include "Measures.h"

#include "TGraph.h"
#include "TCanvas.h"
#include "TAxis.h"
#include "TH1D.h"

extern int debug;
extern int print;
extern string path;
extern string name;
extern string name_print;

using namespace std;

double l_min = -1;
double l_max = 5;
void Draw_on_Canvas(const Measurements &Meas, TGraph& graph, TCanvas& can, string Title, string X_label, string Y_label, int MS = 1, int MC = 4){
    if(debug){cout<<"Draw_on_Canvas Begins"<<endl;}

    vector<Measure> Data;
    Data = Meas.GetData();
    int N_data = Data.size();
    for(int i = 0; i<N_data; i++){
        graph.SetPoint(i, Data[i].GetLambda(), Data[i].GetValue());
    }
    graph.SetMaximum(l_max);
    graph.SetMinimum(l_min);
    graph.SetMarkerColor(MC);
    graph.SetMarkerStyle(MS);
    graph.SetTitle(Title.c_str());
    graph.GetXaxis()->SetTitle(X_label.c_str());
    graph.GetYaxis()->SetTitle(Y_label.c_str());

    can.cd();
    graph.Draw("AP");
    if(print){
        can.Print((path + "out_" + name_print + ".pdf").c_str(), (path + "out_" + name_print + ".pdf[").c_str());
    }
    if(debug){cout<<"Draw_on_Canvas Ends"<<endl;}
};



#endif