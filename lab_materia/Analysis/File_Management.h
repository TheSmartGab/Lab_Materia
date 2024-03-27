#ifndef __File_Management_h__
#define __File_Management_h__


#include <sstream>
#include <iostream>
#include <fstream>
#include <cmath>
#include <vector>
#include "Measures.h"

#ifndef __Debug__
#define __Debug__
static int debug = false;
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

void Print(vector<Measure> Data, ofstream *out){
    if(debug){cout<<"Print Begins"<<endl;}

    *out<<"#"<<endl; //used to sign begin of data
    for(int i = 0; i<Data.size(); i++){
        *out<<Data[i]._dati[0]<<"\t"<<Data[i]._dati[1]<<endl;
    }

    if(debug){cout<<"Print Ends"<<endl;}
}

#endif