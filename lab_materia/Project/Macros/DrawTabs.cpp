#include "TabValues.h"

#include "TCanvas.h"
#include "TGraph.h"
#include "TAxis.h"
#include "TApplication.h"
#include "TF1.h"

int debug = 0;
int print = 0;
int fit = 0;

string path = "../Tab_Data/";
string name1 = "Johnson.txt";
string name2 = "Lemarchand.txt";
string name3 = "Cielsky.txt";
string name4 = "Rosenblat.txt";
string name5 = "Yakubonsky.txt";
string name6 = "Palik.txt";
string name_print;

int main(int argc, const char **argv){



    for(int i = 1; i<argc; i++){
    
        if(!strcmp(argv[i], "-debug")){debug = atoi(argv[++i]);}
        if(!strcmp(argv[i], "-print")){print = atoi(argv[++i]);}
        if(!strcmp(argv[i], "-fit")){fit = atoi(argv[++i]);}        
    }
    
    cout<<"======================================================="<<endl;
    cout<<"Running Configuration"<<endl;
    cout<<"path:\t"<<path<<endl;   
    cout<<"name1:\t"<<name1<<endl;
    cout<<"name2:\t"<<name2<<endl;
    cout<<"name3:\t"<<name3<<endl;
    cout<<"name4:\t"<<name4<<endl;
    cout<<"name5:\t"<<name5<<endl;
    cout<<"name6:\t"<<name6<<endl;
    cout<<"debug:\t"<<debug<<endl;
    cout<<"print:\t"<<print<<endl;
    cout<<"fit:\t"<<fit<<endl;
    cout<<"======================================================="<<endl<<endl;

    if(debug){cout<<"Main Starts"<<endl;}

    //Initialising main
    TApplication myApp("myApp",0,0);
    name_print = "Tabs";

    if(print){
        TCanvas can;
        can.Print((path + "out_" + name_print + ".pdf[").c_str(), (path + "out_" + name_print + ".pdf[").c_str());
    }
    int fit_grade = 8;
    vector<double> params;
    string expr="pol"+to_string(fit_grade);

    double JB[] = {280, 420, 590, 1000};
    TabValues Johnson(20, JB, 4);
    Johnson.DoAll(path, name1, expr, params, 2, 3);

    double PB[] = {280, 380, 460, 600, 1000};
    TabValues Palik(5, PB, 5);
    Palik.DoAll(path, name6, expr, params, 4, 5);

    double LB[] = {300, 400, 480, 600, 1000};
    TabValues Lemachard(6, LB, 5);
    Lemachard.DoAll(path, name2, expr, params, 3, 4, 2, 2, 0, 1);

    double CB[] = {280, 320, 380, 460, 515, 600, 690, 790, 1000};
    TabValues Cielsky(7, CB, 9);
    Cielsky.DoAll(path, name3, expr, params);

    double RB[] = {300, 350, 400, 450, 500, 550, 650, 1000};
    TabValues Rosenblat(6, RB, 8);
    Rosenblat.DoAll(path, name4, expr, params, 2, 3, 2, 2, 0, 1);

    double YV[] = {300, 400, 500, 600, 800, 1000};
    TabValues Yakubonsky(5, JB, 6);
    Yakubonsky.DoAll(path, name5, expr, params, 4, 5);


    TCanvas All_can;
    All_can.cd();
    auto frame = All_can.DrawFrame(250, 0., 1000., 8.);
    All_can.Update();
    Johnson.DrawCanvas(All_can, "P same", 0.7, 1, 0.8, 1);
    Palik.DrawCanvas(All_can, "P same", 0.7, 1, 0.6, 0.8);

    if(print){
        All_can.cd();
        All_can.Print((path + "out_" + name_print + ".pdf").c_str(), (path + "out_" + name_print + ".pdf").c_str());
    }

    //Lemachard.DrawCanvas(All_can, "P same");
    //Cielsky.DrawCanvas(All_can, "P same");
    //Rosenblat.DrawCanvas(All_can, "P same");
    //Yakubonsky.DrawCanvas(All_can, "P same");

    if(print){
        TCanvas can;
        can.Print((path + "out_" + name_print + ".pdf]").c_str(), (path + "out_" + name_print + ".pdf[").c_str());
    }

    cout<<"Running App"<<endl;
    myApp.Run();
    if(debug){cout<<"Main Ends"<<endl;}
    return 0;
}