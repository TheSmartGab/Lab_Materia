#include "Measures.h"
#include <iostream>
#include <vector>

#include "TCanvas.h"
#include "TGraph.h"
#include "TAxis.h"
#include "TApplication.h"
#include "TF1.h"
#include "TMultiGraph.h"

using namespace std;

//bias file
// static string path_bias("../Lab_Data/20_03_24/");
// static string name_bias("200324_aria_aria_1.txt");

int debug = 0;
int print = 0;
int fit = 0;

int noApp = 0;

string path;
string name_print;

string name;

int main(int argc, const char** argv){

    string run;
    TApplication myApp("myApp",0,0);
    cout<<"======================================================="<<endl;
    cout<<"Running Configuration"<<endl;
    for(int i = 1; i<argc; i++){
    
        if(!strcmp(argv[i], "-Run")){string appo(argv[++i]); run = appo;}
        if(!strcmp(argv[i], "-debug")){debug = atoi(argv[++i]);}
        if(!strcmp(argv[i], "-print")){print = atoi(argv[++i]);}
        if(!strcmp(argv[i], "-noApp")){noApp = atoi(argv[++i]);}
    }
    cout<<"debug:\t"<<debug<<endl;
    cout<<"print:\t"<<print<<endl;
    cout<<"noApp:\t"<<noApp<<endl;
    cout<<"======================================================="<<endl<<endl;
    if(debug){cout<<"Main Starts"<<endl;}

    name_print = run;
    path = "../Run_Data/"+run+"/";

    if(print){
        TCanvas can;
        can.cd();
        can.Print((path + "out_" + name_print + ".pdf[").c_str(), (path + "out_" + name_print + ".pdf[").c_str());
    }
    TabValues Johnson;
    Johnson.ReadTabData("../Tab_Data/Johnson.txt");
    Johnson.ScaleLambda(1000);
    Johnson.UpdateART();

    TabValues Palik;
    Palik.ReadTabData("../Tab_Data/Palik.txt");
    Palik.ScaleLambda(1000);
    Palik.UpdateART();

    Run MyRun;
    MyRun.Init(run);
    MyRun.SetAll();

    //fitting thickness
    //MyRun.FitAllThick(Palik);

    MyRun.DrawAll();
    MyRun.DrawAllSame("P same", 0, 0.5, 300, 900);
    MyRun.DrawInner("P same", 0, 0.5, 300, 900);
    MyRun.DrawOuter("P same", 0, 0.5, 300, 900);
    MyRun.DrawAdditional("P same", 0, 0.5, 300, 900);


    if(print){
        TCanvas can;
        can.cd();
        can.Print((path + "out_" + name_print + ".pdf]").c_str(), (path + "out_" + name_print + ".pdf]").c_str());
    }

    if(!noApp){
        cout<<"Running app"<<endl;
        myApp.Run();
    }

    return 0;
}