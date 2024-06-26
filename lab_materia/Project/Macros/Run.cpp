#include "Run.h"
#include "Measurements.h"
#include "TabValues.h"
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
    int add = 0;
    TApplication myApp("myApp",0,0);
    cout<<"======================================================="<<endl;
    cout<<"Running Configuration"<<endl;
    for(int i = 1; i<argc; i++){
    
        if(!strcmp(argv[i], "-Run")){string appo(argv[++i]); run = appo;}
        if(!strcmp(argv[i], "-debug")){debug = atoi(argv[++i]);}
        if(!strcmp(argv[i], "-print")){print = atoi(argv[++i]);}
        if(!strcmp(argv[i], "-noApp")){noApp = atoi(argv[++i]);}
        if(!strcmp(argv[i], "-add")){add = atoi(argv[++i]);}
    }
    cout<<"debug:\t"<<debug<<endl;
    cout<<"print:\t"<<print<<endl;
    cout<<"noApp:\t"<<noApp<<endl;
    cout<<"add\t"<<add<<endl;
    cout<<"======================================================="<<endl<<endl;
    if(debug){cout<<"Main Starts"<<endl;}

    name_print = run;
    path = "../Run_Data/"+run+"/";

    if(print){
        TCanvas can;
        can.cd();
        can.Print((path + "out_" + name_print + ".pdf[").c_str(), (path + "out_" + name_print + ".pdf[").c_str());
    }

    int fit_grade = 8;
    vector<double> params;
    string expr="";
    for(int i=0; i<=fit_grade; i++){
        params.push_back(0);
        expr += ("["+to_string(i)+"]*x^"+to_string(i));
        if(i != fit_grade){expr += "+";}
    }

    setDefaultMinimizerOptions();

    // double JB[] = {280, 420, 590, 1000};
    // TabValues *Johnson = new TabValues(6, JB, 4);
    // Johnson->DoAll("../Tab_Data/", "Johnson.txt", expr, params);
    // Johnson->Close();

    // double PB[] = {280, 380, 460, 600, 1000};
    // TabValues *Palik = new TabValues(6 ,PB, 5);
    // Palik->DoAll("../Tab_Data/", "Palik.txt", expr, params);
    // Palik->Close();



    // double CB[] = {280, 340, 400, 465, 520, 600, 690, 790, 1000};
    // TabValues *Cielsky = new TabValues(6, CB, 9);
    // Cielsky->DoAll("../Tab_Data/", "Cielsky.txt", expr, params);

    // double RB[] = {300, 350, 400, 450, 500, 550, 650, 1000};
    // TabValues *Rosenblat = new TabValues(6, RB, 8);
    // Rosenblat->DoAll("../Tab_Data/", "Rosenblat.txt", expr, params);

    // double LB[] = {280, 350, 450, 550, 1000};
    // TabValues *Lemachard = new TabValues(6, LB, 5);
    // Lemachard->DoAll("../Tab_Data/", "Lemarchand.txt",  expr, params, 3, 4, 2, 2, 0, 1);

    double OB[] = {300, 400, 500, 600, 800, 1000};
    TabValues *Olmon = new TabValues(5, OB, 6);
    Olmon->DoAll("../Tab_Data/", "Olmon.txt", expr, params, 4, 6, 2, 2, 0, 1);

    Run *MyRun = new Run();
    MyRun->Init(run);
    MyRun->SetAll(300, 900, 0, 1);

    //fitting thickness
    MyRun->FitAllThick(*Olmon, 320, 480.);
    MyRun->SetNameFitter("Olmon");

    MyRun->DrawAll();
    //MyRun->CloseAllCan();
    MyRun->CloseRaws();
    MyRun->CloseCorrected();
    MyRun->DrawInner("P same", 0, 1, 300, 900);
    MyRun->CloseInner();

    MyRun->DrawOuter("P same", 0, 1, 300, 900);
    MyRun->CloseOuter();

    MyRun->DrawAdditional("P same", 0, 1, 300, 900);
    MyRun->CloseAdditional();
    
    MyRun->DrawAllSame("P same", 0, 1, 300, 900); //by putting this last, we ensure that all the legends are visible

    MyRun->SetTD();
    MyRun->DoAllGraphs();

    MyRun->UpdateAll();

    if(print){
        TCanvas can;
        can.cd();
        can.Print((path + "out_" + name_print + ".pdf]").c_str(), (path + "out_" + name_print + ".pdf]").c_str());
    }

    //appending fitted thickness to a file
    // Open a file in append mode

    if(add){    
        ofstream RunOut(("../Run_Data/"+run+"/Thickness_Fitted.txt"), ios_base::app);
        if (!RunOut.is_open()) { // Corrected check
            cout << "Error: Unable to open file for appending."<<endl;
        }
        // Append the content of TDistribution to the file
        MyRun->Print(RunOut);
        // Close the file
        RunOut.close(); // Corrected close

        ofstream outFile("../Normalized_Thickness/distribution_data.txt", ios_base::app);
        if (!outFile.is_open()) {
            cout << "Error: Unable to open file for appending."<<endl;
        }
        // Append the content of TDistribution to the file
        MyRun->NormalizeThickness();
        MyRun->Print(outFile);
        // Close the file
        outFile.close();
        }

    //Note that delete commands are executed even after a Ctr+C
    if(!noApp){
        cout<<"Running app"<<endl;
        myApp.Run();
    }
    delete MyRun; MyRun = 0x0;

    // delete Johnson; Johnson = 0x0;
    // delete Palik; Palik = 0x0;
    // delete Cielsky; Cielsky = 0x0;
    //delete Rosenblat; Rosenblat = 0x0;
    // delete Lemachard; Lemachard = 0x0;
    delete Olmon; Olmon = 0x0;
    return 0;
}