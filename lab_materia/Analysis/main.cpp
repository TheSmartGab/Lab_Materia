#include "Measures.h"
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

string path;
string name;
string name_print;

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
        if(!strcmp(argv[i], "-name")){string name_appo(argv[++i]); name = name_appo + ".txt"; name_print = name_appo; cout<<"name: "<<name<<endl;}
        if(!strcmp(argv[i], "-debug")){debug = atoi(argv[++i]);}
        if(!strcmp(argv[i], "-print")){print = atoi(argv[++i]);}
        
    }
    cout<<"debug:\t"<<debug<<endl;
    cout<<"print:\t"<<print<<endl;
    cout<<"path_bias:\t"<<path_bias<<endl;
    cout<<"name_bias:\t"<<name_bias<<endl;
    cout<<"======================================================="<<endl<<endl;
    if(debug){cout<<"Main Starts"<<endl;}

    //data file by command line
    ofstream out((path+"out_" + name).c_str());
    if(print){
        TCanvas can;
        can.Print((path + "out_" + name_print + ".pdf[").c_str(), (path + "out_" + name_print + ".pdf[").c_str());
    }
    
    //bias data
    Measurements Bias;
    Bias.ReadAllData((path_bias+name_bias).c_str());

    
    //Data
    Measurements Data;
    Data.ReadAllData((path+name).c_str());
    Data.Print(&out);
    Data.SetGraph("Data", "lambda", "Transmittance", 8, 4, "Data");
    Data.Draw("AP");

    Bias.SetBias(Data);
    Bias.SetGraph("Bias" , "lambda", "Transmittance", 8, 4, "Bias" );
    Bias.Draw("AP");

    //Correct_Data
    Measurements Correct_Data;
    Correct_Data.Correct(Data, Bias);
    Correct_Data.Print(&out);
    Correct_Data.SetGraph("Corrected_Data", "lambda", "Transmittance", 8, 4, "Corrected Data");
    Correct_Data.Draw("AP");

    //fitting bias data
    /*
    TF1 func("func", "[0]*exp(-[1]/x)", 200., 900.);
    func.SetParameters(-2E20, -10000, 50000);
    func.SetParLimits(1, 1000, 50000);
    bias_graph.Fit("func", "+");
    */

    //CLosing actions
    out.close();
    if(print){
        TCanvas can;
        can.Print((path + "out_" + name_print + ".pdf]").c_str(), (path + "out_" + name_print + ".pdf[").c_str());
    }

    cout<<"Running App"<<endl;
    myApp.Run();
    if(debug){cout<<"Main Ends"<<endl;}
    return 0;
}