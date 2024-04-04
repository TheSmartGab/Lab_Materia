#include "File_Management.h"
#include "Measures.h"
#include <iostream>
#include <fstream>

#include "TCanvas.h"
#include "TGraph.h"
#include "TAxis.h"
#include "TApplication.h"

using namespace std;

int main(int argc, const char** argv){
    //Draw the histogram with the differences between 2 Spectra
    //It was designed to check that changing the calibration wl truly only induced a shift


    //Initialising main
    TApplication myApp("myApp",0,0);
    string path_1;
    string name_1;
    string path_2;
    string name_2;
    string name_print_1;
    string name_print_2;

    if(debug){cout<<"Main Begins"<<endl;}    

    cout<<"======================================================="<<endl;
    cout<<"Running Configuration"<<endl;
    for(int i = 1; i<argc; i++){
    
        if(!strcmp(argv[i], "-path_1")){string path_appo(argv[++i]); path_1 = path_appo; cout<<"path_1: "<<path_1<<endl; }
        if(!strcmp(argv[i], "-name_1")){string name_appo(argv[++i]); name_1 = name_appo + ".txt"; cout<<"name_1: "<<name_1<<endl;name_print_1 = name_appo;}
        if(!strcmp(argv[i], "-path_2")){string path_appo(argv[++i]); path_2 = path_appo; cout<<"path_2: "<<path_2<<endl;}
        if(!strcmp(argv[i], "-name_2")){string name_appo(argv[++i]); name_2 = name_appo + ".txt"; cout<<"name_2: "<<name_2<<endl;name_print_2 = name_appo;} 
        if(!strcmp(argv[i], "-debug")){debug = atoi(argv[++i]);}
        if(!strcmp(argv[i], "-print")){print = atoi(argv[++i]);}
        
    }
    path = path_1;
    name_print = name_print_1 + "-" + name_print_2;
    cout<<"debug:\t"<<debug<<endl;
    cout<<"print:\t"<<print<<endl;
    cout<<"======================================================="<<endl<<endl;
    if(debug){cout<<"Main Starts"<<endl;}

    //data file by command line
    ofstream out((path_1+"diff_" + name_print_1 +"-" + name_print_2+".txt").c_str());

    if(print){
        TCanvas can;
        can.Print((path + "out_" + name_print + ".pdf[").c_str(), (path + "out_" + name_print + ".pdf[").c_str());
    }
    vector<Measure> Data_1;
    ReadAllData((path_1 + name_1).c_str(), Data_1);
    TGraph graph_1;
    TCanvas can_1;
    Draw_on_Canvas(Data_1, graph_1, can_1, "Data_1", "lambda", "Trasmittance");
    
    vector<Measure> Data_2;
    ReadAllData((path_2 + name_2).c_str(), Data_2);
    TGraph graph_2;
    TCanvas can_2;
    Draw_on_Canvas(Data_2, graph_2, can_2, "Data_2", "lambda", "Trasmittance");

    vector<Measure> Diff = Difference(Data_1, Data_2);
    Print(Diff, &out);
    TGraph graph_diff;
    TCanvas can_diff;
    Draw_on_Canvas(Diff, graph_diff, can_diff, "Difference (1-2)", "lambda", "Difference");


    out.close();
    if(print){
        TCanvas can;
        can.Print((path + "out_" + name_print + ".pdf]").c_str(), (path + "out_" + name_print + ".pdf[").c_str());
    }

    cout<<"Running app"<<endl;
    myApp.Run();
    if(debug){cout<<"Main Ends"<<endl;}
    return 0;
}