#include "Info.h"


void Glass_Info::Read(stringstream &ss){
    ss>>ID; ss>>Pos;
};

void Run_Info::RI_FromFile(const char* Filename){

    if(debug){cout<<"RI_FromFile Starts, filename = "<<Filename<<endl;}

    ifstream in(Filename);
    if(!in){cout<<"\nImpossibile aprire il file \""<<Filename<<"\""<<endl; exit(-1); }

    bool start_add = false;

    do{
        string str;
        getline(in, str);
        stringstream ss(str);
        string value = "";
        TString Tstr = str;
        string type = "";
        Glass_Info appo;

        if(start_add){
            appo.SetType(type);
            appo.Read(ss);
            Additional.push_back(appo);

        }

        while(ss >> value){
            if(value == "Type:"){ss>>type;}
            if(value == "ID"){
                ss>>value;
                if(value == "R[m]"){
                    start_add = true;
                }
            }
            if(value == "end" && start_add){start_add = false;}
        }
        
    }while(!in.eof());
    
    in.seekg(0);
    in.close();

    if(debug){cout<<"RI_FromFile Ends, filename = "<<Filename<<endl;}
};

void Disk_Geo::DG_FromFile(const char* Filename){

    if(debug){cout<<"DG_FromFile Starts, filename = "<<Filename<<endl;}

    ifstream in(Filename);
    if(!in){cout<<"\nImpossibile aprire il file \""<<Filename<<"\""<<endl; exit(-1); }

    bool start_add = false;

    do{
        string str;
        getline(in, str);
        stringstream ss(str);
        string value = "";
        TString Tstr = str;
        string type = "";
        Glass_Info appo;

        if(start_add){
            if(type == "Center"){
                appo.SetType(type);
                appo.Read(ss);
                Center.push_back(appo);
            }
            if(type == "Inner"){
                appo.SetType(type);
                appo.Read(ss);
                Inner.push_back(appo);
            }
            if(type == "Outer"){
                appo.SetType(type);
                appo.Read(ss);
                Outer.push_back(appo);
            }
        }

        while(ss >> value){
            if(value == "Type:"){ss>>type;}
            if(value == "ID"){
                ss>>value;
                if(value == "R[m]"){
                    start_add = true;
                }
            }
            if(value == "end" && start_add){start_add = false;}
        }
        
    }while(!in.eof());
    
    in.seekg(0);
    in.close();

    if(debug){cout<<"DG_FromFile Ends, filename = "<<Filename<<endl;}
};