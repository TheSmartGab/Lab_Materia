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
#include "TLegend.h"
#include "TMultiGraph.h"

extern int debug;
extern int print;
extern int fit;
extern string path;
extern string name_print;

using namespace std;

double l_min = -1;
double l_max = 5;
void Draw_on_Canvas(Measurements &Meas, TGraph& graph, TCanvas& can, string Title, string X_label, string Y_label, int MS = 1, int MC = 4, string opt = ""){
    if(debug){cout<<"Draw_on_Canvas Begins"<<endl;}

    vector<Measure> Data;
    Data = Meas.GetData();
    int N_data = Data.size();
    for(int i = 0; i<N_data; i++){
        graph.SetPoint(i, Data[i].GetLambda(), Data[i].GetValue());
    }
    cout<<"debug"<<endl;
    graph.SetMaximum(l_max);
    graph.SetMinimum(l_min);
    graph.SetMarkerColor(MC);
    graph.SetMarkerStyle(MS);
    graph.SetTitle(Title.c_str());
    graph.GetXaxis()->SetTitle(X_label.c_str());
    graph.GetYaxis()->SetTitle(Y_label.c_str());

    can.cd();
    graph.Draw(("AP" + opt).c_str());

    if(print){
        can.Print((path + "out_" + name_print + ".pdf").c_str(), (path + "out_" + name_print + ".pdf[").c_str());
    }
    if(debug){cout<<"Draw_on_Canvas Ends"<<endl;}
};

void Draw_on_Canvas(TabValues &Tab, TGraph& graphn, TGraph& graphk, TCanvas& can, string Title, string X_label, string Y_label, int MS =8, int MCn = 4, int MCk = 5, bool clear = true){
    if(debug){cout<<"Draw_on_Canvas Begins"<<endl;}

    Tab.SetGraphK(graphk, MS, MCk);
    Tab.SetGraphN(graphn, MS, MCn);

    graphn.GetXaxis()->SetTitle(X_label.c_str());
    graphn.GetYaxis()->SetTitle(Y_label.c_str());
    graphn.SetMarkerSize(0.5);

    graphk.SetMarkerSize(0.5);

    can.cd();
    if(clear){auto frame = can.DrawFrame(0, 0, 2, 14);}
    graphk.Draw("P same");
    graphn.Draw("P same");

   auto legend = new TLegend(0.1,0.7,0.48,0.9);
   legend->AddEntry(&graphn,("n " + Title).c_str(),"P");
   legend->AddEntry(&graphk,("k " + Title).c_str(),"P");
   legend->Draw();

    if(print){
        can.Print((path + "out_" + name_print + ".pdf").c_str(), (path + "out_" + name_print + ".pdf[").c_str());
    }
    if(debug){cout<<"Draw_on_Canvas Ends"<<endl;}
};



void Draw_vector_Tab(TMultiGraph &mg, vector<TabValues> &Tabs, vector<TGraph>& graphsn, vector<TGraph>& graphsk, TCanvas& can, const vector<string> Titles, string X_label, string Y_label, int MS, const vector<int>& MCn, const vector<int>& MCk){

    TLegend *legend = new TLegend(0.1,0.7,0.48,0.9);
    int N_Tabs = Tabs.size();
    for(int i=0; i<N_Tabs; i++){
        Tabs[i].SetGraphN(graphsn[i], MS, MCn[i]);
        Tabs[i].SetGraphK(graphsk[i], MS, MCk[i]);
        legend->AddEntry(& graphsn[i], ("n" + Titles[i]).c_str(), "P");
        legend->AddEntry(& graphsk[i], ("k" + Titles[i]).c_str(), "P");
        mg.Add(&graphsn[i]);
        mg.Add(&graphsk[i]);
    }

    auto frame = can.DrawFrame(0, 0, 2, 14);
    mg.GetXaxis()->SetTitle(X_label.c_str());
    mg.GetYaxis()->SetTitle(Y_label.c_str());
    can.cd();
    mg.Draw("AP same");
    legend->Draw();
    can.Update();

    if(print){
        can.Print((path + "out_" + name_print + ".pdf").c_str(), (path + "out_" + name_print + ".pdf[").c_str());
    }
    if(debug){cout<<"Draw_on_Canvas Ends"<<endl;}
}




#endif