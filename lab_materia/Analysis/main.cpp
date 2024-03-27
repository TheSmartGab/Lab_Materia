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

//bias file
string path_bias("../Lab_Data/20_03_24/");
string name_bias("200324_aria_aria_1.txt");

int main(int argc, const char** argv){
    //Read the file given, corrects it for the bias by default and plot bias and corrected data
    //corrected data are also printed to a file with same name & path of input file + out_ prepended

    //Initialising main
    TApplication myApp("myApp",0,0);
    string path;
    string name;
    int print = 0;

    for(int i = 1; i<argc; i++){
        cout<<argv[i]<<endl;
    
        if(!strcmp(argv[i], "-path")){string path_appo(argv[++i]); path = path_appo; cout<<"path: "<<path<<endl;}
        if(!strcmp(argv[i], "-name")){string name_appo(argv[++i]); name = name_appo; cout<<"name: "<<name<<endl;}
        if(!strcmp(argv[i], "-debug")){debug = atoi(argv[++i]);}
        if(!strcmp(argv[i], "-print")){print = atoi(argv[++i]);}
        
    }
    cout<<"debug: "<<debug<<endl;
    cout<<"print: "<<print<<endl;
    if(debug){cout<<"Main Starts"<<endl;}

    //data file by command line
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
    TGraph Raw_graph;
    Raw_graph.SetTitle("Raw Data Measurements");
    Raw_graph.GetXaxis()->SetTitle("lambda");
    for(int i = 0; i<Data.size();i++){
        Raw_graph.SetPoint(i,Data[i]._dati[0], Data[i]._dati[1]);
    }
    Raw_graph.SetMarkerColor(4);


    vector<Measure> Correct_Data;
    Correct(Data, &Correct_Data, Bias);
    Print(Correct_Data, &out);

    TGraph Correct_graph;
    Correct_graph.SetTitle("Correct Data Measurements");
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
    if(print){cout<<"Printing ... "<<endl; bias_can.Print(("(" + path + name +".pdf").c_str() , "pdf");}

    TCanvas correct_can;
    correct_can.cd();
    Correct_graph.Draw("A*");

    TCanvas Raw_can;
    Raw_can.cd();
    Raw_graph.Draw("A*");

    //CLosing actions
    out.close();

    cout<<"Running App"<<endl;
    myApp.Run();
    if(debug){cout<<"Main Ends"<<endl;}
    return 0;
}