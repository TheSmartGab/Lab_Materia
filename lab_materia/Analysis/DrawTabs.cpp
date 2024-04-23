#include "Measures.h"

#include "TCanvas.h"
#include "TGraph.h"
#include "TAxis.h"
#include "TApplication.h"
#include "TF1.h"

int debug = 0;
int print = 0;
int fit = 0;

string path = "../Tab_Data/";
string name1;
string name2;
string name3;
string name4;
string name5;
string name6;
string name_print;

int main(int argc, const char **argv){


    for(int i = 1; i<argc; i++){
    
        if(!strcmp(argv[i], "-name1")){string name_appo(argv[++i]); name1 = name_appo + ".txt"; name_print += ("_"+name_appo+"_");}
        if(!strcmp(argv[i], "-name2")){string name_appo(argv[++i]); name2 = name_appo + ".txt"; name_print += ("_"+name_appo+"_");}      
        if(!strcmp(argv[i], "-name3")){string name_appo(argv[++i]); name3 = name_appo + ".txt"; name_print += ("_"+name_appo+"_");}
        if(!strcmp(argv[i], "-name4")){string name_appo(argv[++i]); name4 = name_appo + ".txt"; name_print += ("_"+name_appo+"_");}
        if(!strcmp(argv[i], "-name5")){string name_appo(argv[++i]); name5 = name_appo + ".txt"; name_print += ("_"+name_appo+"_");}
        if(!strcmp(argv[i], "-name6")){string name_appo(argv[++i]); name6 = name_appo + ".txt"; name_print += ("_"+name_appo+"_");}
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
    string expr="";
    for(int i=0; i<=fit_grade; i++){
        params.push_back(0);
        expr += ("["+to_string(i)+"]*x^"+to_string(i));
        if(i != fit_grade){expr += "+";}
    }

    TabValues Johnson;
    Johnson.DoAll(path, name1, expr, params, 2, 3);

    TabValues Palik;
    Palik.DoAll(path, name6, expr, params, 4, 5);

    TabValues Lemachard;
    Lemachard.DoAll(path, name2, expr, params);

    TabValues Cielsky;
    Cielsky.DoAll(path, name3, expr, params);

    TabValues Rosenblat;
    Rosenblat.DoAll(path, name4, expr, params, 2, 3);

    TabValues Yakubonsky;
    Yakubonsky.DoAll(path, name5, expr, params, 4, 5);


    TCanvas All_can;
    All_can.cd();
    auto frame = All_can.DrawFrame(0., 0., 2000., 14.);
    All_can.Update();
    Johnson.DrawCanvas(All_can, "P same");
    Palik.DrawCanvas(All_can, "P same");
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