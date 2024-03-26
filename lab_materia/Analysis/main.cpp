#include "File_Management.h"
#include "Measures.h"
#include <iostream>
#include <vector>

#include "TCanvas.h"
#include "TGraph.h"
#include "TAxis.h"
#include "TApplication.h"
#include "TF1.h"

using namespace std;

#ifndef __Debug__
#define __Debug__
static int debug = false;
#endif

int main(int argc, char** argv){
    //Read the file given, corrects it for the bias by default and plot bias and corrected data
    //corrected data are also printed to a file with same name & path of input file + out_ prepended
    if(argc < 3 || argc >4){
        cout<<"Program Usage : ./main <path> <filename> <debug>"<<endl;
        exit(-1);
    }

    //Initialising main
    TApplication myApp("myApp",0,0);
    if(argc == 4){debug = atoi(argv[3]);}
    if(debug){cout<<"Main Starts"<<endl;}

    //bias file
    string path_bias("../Lab_Data/20_03_24/");
    string name_bias("200324_aria_aria_1.txt");

    //data file by command line
    string path(argv[1]);
    string name(argv[2]);
    ofstream out((path+"out_" + name));
    
    //bias data
    vector<Measure> Bias;
    ReadAllData((path_bias+name_bias).c_str(), Bias);

    TGraph bias_graph;
    bias_graph.SetTitle("Calibration Measurements");
    bias_graph.GetXaxis()->SetTitle("lambda");
    for(int i = 0; i<Bias.size();i++){
        bias_graph.SetPoint(i, Bias[i]._dati[0], Bias[i]._dati[1]);
    }
    bias_graph.SetMarkerColor(4);

    //data
    vector<Measure> Data;
    ReadAllData((path+name).c_str(), Data);
    vector<Measure> Correct_Data;
    Correct(Data, &Correct_Data, Bias);
    Print(Correct_Data, &out);

    TGraph Correct_graph;
    Correct_graph.SetTitle("Data Measurements");
    Correct_graph.GetXaxis()->SetTitle("lambda");
    for(int i = 0; i<Correct_Data.size();i++){
        Correct_graph.SetPoint(i, Correct_Data[i]._dati[0], Correct_Data[i]._dati[1]);
    }
    Correct_graph.SetMarkerColor(4);


    //fitting bias data
    /*
    TF1 func("func", "[0]*exp(-[1]/x)", 200., 900.);
    func.SetParameters(-2E20, -10000, 50000);
    func.SetParLimits(1, 1000, 50000);
    bias_graph.Fit("func", "+");
    */

    TCanvas bias_can;
    bias_can.cd();
    bias_graph.Draw("A*");

    TCanvas correct_can;
    correct_can.cd();
    Correct_graph.Draw("A*");

    //CLosing actions
    out.close();

    cout<<"Running App"<<endl;
    myApp.Run();
    if(debug){cout<<"Main Ends"<<endl;}
    return 0;
}