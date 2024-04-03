#ifndef __File_Management_h__
#define __File_Management_h__


#include <sstream>
#include <iostream>
#include <fstream>
#include <cmath>
#include <vector>
#include "Measures.h"

#include "TGraph.h"
#include "TCanvas.h"
#include "TAxis.h"
#include "TH1D.h"

#ifndef __Debug__
#define __Debug__
static int debug = 0;
#endif

#ifndef __Print__
#define __Print__
static int print = 0;
#endif

#ifndef __Files__
#define __Files__
static string path;
static string name;
static string name_print;
#endif

using namespace std;

void ReadAllData(const char* filename, vector<Measure> &dati){

    if(debug){cout<<"ReadALlData Starts, filename = "<<filename<<endl;}

    ifstream in(filename);
    if(!in){cout<<"\nImpossibile aprire il file \""<<filename<<"\""<<endl; exit(-1); }

    bool start = false;
    do{
        string arr;
        getline(in, arr);
        stringstream ss(arr);
        Measure appo;
        string value = "";

        while(ss >> value){
            if(value == "#"){start = true;}
            appo._dati.push_back(atof(value.c_str()));
        }
        
        if(appo._dati.size() == 2 && start == true) dati.push_back(appo);
    }while(!in.eof());


    if(start == false){ //txt files contains foo numbers, rather then eliminating them, add an # before the usefull datas
        cout<<"Error while loading "<<filename<<": missing #"<<endl;
        exit(-1);
    }


    if(debug){
        cout<<"Data Uploaded:"<<endl;
        for(int i = 0; i<dati.size(); i++){
            cout<<dati[i]._dati[0]<<"\t"<<dati[i]._dati[1]<<endl;
            cout<<dati[i]._dati[0]<<"\t"<<dati[i]._dati[1]<<endl;
        }
    }
    
    in.seekg(0);
    in.close();

    if(debug){cout<<"ReadALlData Ends, filename = "<<filename<<endl;}
};

void ReadTabData(const char* filename, vector<Measure> &dati_n, vector<Measure> &dati_k){

    if(debug){cout<<"ReadTabData Starts, filename = "<<filename<<endl;}

    ifstream in(filename);
    if(!in){cout<<"\nImpossibile aprire il file \""<<filename<<"\""<<endl; exit(-1); }

    bool start_n = false;
    bool start_k = false;
    do{
        string arr;
        getline(in, arr);
        stringstream ss(arr);
        Measure appo;
        string value = "";

        while(ss >> value){
            if(value == "n"){start_n = true; start_k = false;}
            if(value == "k"){start_k = true; start_n = false;}
            appo._dati.push_back(atof(value.c_str()));
        }
        
        if(appo._dati.size() == 2 && start_n == true){dati_n.push_back(appo);}
        if(appo._dati.size() == 2 && start_k == true){dati_k.push_back(appo);}
    }while(!in.eof());

    if(debug){
        cout<<"TabData Uploaded:"<<endl;
        cout<<"Dati_n:"<<endl;
        for(int i = 0; i<dati_n.size(); i++){
            cout<<dati_n[i]._dati[0]<<"\t"<<dati_n[i]._dati[1]<<endl;
            cout<<dati_n[i]._dati[0]<<"\t"<<dati_n[i]._dati[1]<<endl;
        }
        cout<<"Dati_k"<<endl;
        for(int i = 0; i<dati_k.size();i++){
            cout<<dati_k[i]._dati[0]<<"\t"<<dati_k[i]._dati[1]<<endl;
            cout<<dati_k[i]._dati[0]<<"\t"<<dati_k[i]._dati[1]<<endl;
        }
    }
    
    in.seekg(0);
    in.close();

    if(debug){cout<<"ReadALlData Ends, filename = "<<filename<<endl;}
};

void Print(vector<Measure> Data, ofstream *out){
    if(debug){cout<<"Print Begins"<<endl;}

    *out<<"#"<<endl; //used to sign begin of data
    for(int i = 0; i<Data.size(); i++){
        *out<<Data[i]._dati[0]<<"\t"<<Data[i]._dati[1]<<endl;
    }

    if(debug){cout<<"Print Ends"<<endl;}
};

static int MS = 1;
static int MC = 4;
void Draw_on_Canvas(vector<Measure> &Data, TGraph& graph, TCanvas& can, string Title, string X_label, string Y_label){
    //print = 1;
    cout<<print<<endl;
    for(int i = 0; i<Data.size(); i++){
        graph.SetPoint(i, Data[i]._dati[0], Data[i]._dati[1]);
    }
    graph.SetMarkerColor(MC);
    graph.SetMarkerStyle(MS);
    graph.SetTitle(Title.c_str());
    graph.GetXaxis()->SetTitle(X_label.c_str());
    graph.GetYaxis()->SetTitle(Y_label.c_str());

    can.cd();
    graph.Draw("AP");
    cout<<print<<endl;
    if(print){
        can.Print((path + "out_" + name_print + ".pdf").c_str(), (path + "out_" + name_print + ".pdf[").c_str());
    }
};



#endif