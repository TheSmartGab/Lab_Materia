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
string name_print;

string name;
string name1 = "";
string name2 = "";

int main(int argc, const char** argv){
    //Read the file given, corrects it for the bias by default and plot bias and corrected data
    //corrected data are also printed to a file with same name & path of input file + out_ prepended

    //Initialising main
    TApplication myApp("myApp",0,0);

    //Pallick

    cout<<"======================================================="<<endl;
    cout<<"Running Configuration"<<endl;
    for(int i = 1; i<argc; i++){
    
        if(!strcmp(argv[i], "-path")){string path_appo(argv[++i]); path = path_appo; cout<<"path: "<<path<<endl;}
        if(!strcmp(argv[i], "-name1")){string name_appo(argv[++i]); name1 = name_appo + ".txt"; cout<<"name1: "<<name<<endl;}
        if(!strcmp(argv[i], "-name2")){string name_appo(argv[++i]); name2 = name_appo + ".txt"; cout<<"name2: "<<name<<endl;}
        if(!strcmp(argv[i], "-debug")){debug = atoi(argv[++i]);}
        if(!strcmp(argv[i], "-print")){print = atoi(argv[++i]);}
        if(!strcmp(argv[i], "-noApp")){noApp = atoi(argv[++i]);}
        
    }
    cout<<"debug:\t"<<debug<<endl;
    cout<<"print:\t"<<print<<endl;
    cout<<"path_bias:\t"<<path_bias<<endl;
    cout<<"name_bias:\t"<<name_bias<<endl;
    cout<<"noApp:\t"<<noApp<<endl;
    cout<<"======================================================="<<endl<<endl;
    if(debug){cout<<"Main Starts"<<endl;}

    //data file by command line
    ofstream out((path+"out_" + name).c_str());
    if(print){
        TCanvas can;
        can.Print((path + "out_" + name_print + ".pdf[").c_str(), (path + "out_" + name_print + ".pdf[").c_str());
    }
    
    // bias data
    Measurements Bias;
    Bias.ReadAllData((path_bias+name_bias).c_str());
    
    //Data
    Measurements Data1;
    Data.ReadAllData((path+name1).c_str());
    Data.Print();
    Data.SetGraph("Data", "lambda", "Transmittance", 8, 4, -1, 3.5, 300, 900);
    Data.Draw("AP", "Raw_Data1");

    Bias.SetBias(Data1);
    Bias.SetGraph("Bias" , "lambda", "Transmittance", 8, 4, -1, 3.5, 300, 900 );
    Bias.Draw("AP", "Bias");

    //Correct_Data
    Measurements Correct_Data1;
    Correct_Data1.Correct(Data1, Bias);
    Correct_Data1.Print(&out);
    Correct_Data1.SetGraph("Corrected_Data", "lambda", "Transmittance", 8, 4, 0, 1, 300, 900);
    Correct_Data1.Draw("AP", "Corrected Data");

    //Data
    Measurements Data1;
    Data.ReadAllData((path+name1).c_str());
    Data.Print();
    Data.SetGraph("Data", "lambda", "Transmittance", 8, 4, -1, 3.5, 300, 900);
    Data.Draw("AP", "Raw_Data1");

    Bias.SetBias(Data1);
    Bias.SetGraph("Bias" , "lambda", "Transmittance", 8, 4, -1, 3.5, 300, 900 );
    Bias.Draw("AP", "Bias");

    //Correct_Data
    Measurements Correct_Data1;
    Correct_Data1.Correct(Data1, Bias);
    Correct_Data1.Print(&out);
    Correct_Data1.SetGraph("Corrected_Data", "lambda", "Transmittance", 8, 4, 0, 1, 300, 900);
    Correct_Data1.Draw("AP", "Corrected Data");
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