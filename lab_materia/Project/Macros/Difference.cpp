#include "Measurements.h"
#include <iostream>
#include <vector>

#include "TCanvas.h"
#include "TGraph.h"
#include "TAxis.h"
#include "TApplication.h"
#include "TF1.h"

using namespace std;

//bias file
static string path_bias("../Lab_Data/20_03_24/");
static string name_bias("200324_aria_aria_1.txt");

int debug = 0;
int print = 0;
int fit = 0;

int noApp = 0;

string path;
string path2 = "";
string name_print;

string name;
string name1 = "";
string name2 = "";

int main(int argc, const char** argv){
    //Read the file given, corrects it for the bias by default and plot bias and corrected data
    //corrected data are also printed to a file with same name & path of input file + out_ prepended

    //Initialising main
    TApplication myApp("myApp",0,0);

    cout<<"======================================================="<<endl;
    cout<<"Running Configuration"<<endl;
    for(int i = 1; i<argc; i++){
    
        if(!strcmp(argv[i], "-path")){string path_appo(argv[++i]); path = path_appo; cout<<"path: "<<path<<endl;}
        if(!strcmp(argv[i], "-path2")){string path_appo(argv[++i]); path2 = path_appo; cout<<"path2: "<<path2<<endl;}
        if(!strcmp(argv[i], "-name1")){string name_appo(argv[++i]); name1 = name_appo + ".txt"; cout<<"name1: "<<name1<<endl;}
        if(!strcmp(argv[i], "-name2")){string name_appo(argv[++i]); name2 = name_appo + ".txt"; cout<<"name2: "<<name2<<endl;}
        if(!strcmp(argv[i], "-debug")){debug = atoi(argv[++i]);}
        if(!strcmp(argv[i], "-print")){print = atoi(argv[++i]);}
        if(!strcmp(argv[i], "-noApp")){noApp = atoi(argv[++i]);}
        
    }
    name_print = name1 + "-" + name2;

    cout<<"debug:\t"<<debug<<endl;
    cout<<"print:\t"<<print<<endl;
    cout<<"path_bias:\t"<<path_bias<<endl;
    cout<<"name_bias:\t"<<name_bias<<endl;
    cout<<"noApp:\t"<<noApp<<endl;
    cout<<"======================================================="<<endl<<endl;
    if(debug){cout<<"Main Starts"<<endl;}

    //if path2 is not passed (ie, it's ""), make it equal to path
    if(path2 == ""){
        path2 = path;
    }

    //data file by command line
    ofstream out((path+ "Diff_" + name1 + "_" + name2 + ".txt").c_str());
    if(print){
        TCanvas can;
        can.Print((path + "out_" + name_print + ".pdf[").c_str(), (path + "out_" + name_print + ".pdf[").c_str());
    }
    
    // bias data
    Measurements Bias;
    Bias.ReadAllData((path_bias+name_bias).c_str());
    Bias.SetGraph("Bias" , "lambda", "Transmittance", 8, 4, 0, 2, 200, 900 );
    Bias.Draw("AP", "Bias");
    
    //Data
    Measurements Data1;
    Data1.ReadAllData((path+name1).c_str());
    Data1.SetGraph("Data1", "lambda", "Transmittance", 8, 4, 0, 0.5, 200, 900);
    Data1.Draw("AP", "Raw_Data1");

    Bias.SetBias(Data1);


    //Correct_Data
    Measurements Correct_Data1;
    Correct_Data1.Correct(Data1, Bias);
    out<<"Data1:"<<endl;
    Correct_Data1.Print(&out);
    Correct_Data1.SetGraph("Corrected_Data", "lambda", "Transmittance", 8, 4, 0, 0.5, 200, 900);
    Correct_Data1.Draw("AP", "Data1");

    //Data
    Measurements Data2;
    Data2.ReadAllData((path2+name2).c_str());
    Data2.SetGraph("Data2", "lambda", "Transmittance", 8, 4, 0, 0.5, 200, 900);
    Data2.Draw("AP", "Raw_Data2");

    Bias.SetBias(Data2);

    //Correct_Data
    Measurements Correct_Data2;
    Correct_Data2.Correct(Data2, Bias);
    out<<"Data2:"<<endl;
    Correct_Data2.Print(&out);
    Correct_Data2.SetGraph("Corrected_Data", "lambda", "Transmittance", 8, 4, 0, 0.5, 200, 900);
    Correct_Data2.Draw("AP", "Data2");

    //Difference
    Measurements Difference;
    Difference = Data1 - Data2;
    cout << "========================================================="<<endl;
    cout<<"MaxAbsDifference:\t" << Difference.MaxAbs(320, 850);
    cout << "========================================================="<<endl;
    out<<"Data1 - Data2"<<endl;
    Difference.Print(&out);
    Difference.SetGraph("Difference", "lambda[nm]", "Transmittance Difference", 8, 4, -0.1, 0.1, 200, 900);
    Difference.Draw("AP", "Data1 - Data2");

    //CLosing actions
    out.close();
    if(print){
        TCanvas can;
        can.Print((path + "out_" + name_print + ".pdf]").c_str(), (path + "out_" + name_print + ".pdf[").c_str());
    }

    if(!noApp){
        cout<<"Running App"<<endl;
        myApp.Run();
    }
    if(debug){cout<<"Main Ends"<<endl;}
    return 0;
}