#include "Info.h"


void Glass_Info::Read(stringstream &ss){
    ss>>ID; ss>>Rho;
};

void Run_Info::RI_FromFile(const char* Filename){

    if(debug){cout<<"RI_FromFile Starts, filename = "<<Filename<<endl;}

    ifstream in(Filename);
    if(!in){cout<<"\nImpossibile aprire il file \""<<Filename<<"\""<<endl; exit(-1); }

    bool start_add = false;
    string type = "";

    do{
        string str;
        getline(in, str);
        stringstream ss(str);

        string value = "";
        TString Tstr = str;
        Glass_Info appo;

        if(start_add){
            if(value == "end"){start_add = false;}
            else{
                appo.SetType(type);
                appo.Read(ss);
                Additional.push_back(appo);
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
            if(value == "Height[m]:"){ss>>Height;}
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
    string type = "";

    do{
        string str;
        getline(in, str);
        stringstream ss(str);
        string value = "";
        Glass_Info appo;

        if(start_add){
            if(str == "end"){start_add = false;}
            else{
                appo.SetType(type);
                appo.Read(ss);
                if(type == "Center"){
                    Center.push_back(appo);
                }
                if(type == "Inner"){
                    Inner.push_back(appo);
                }
                if(type == "Outer"){
                    Outer.push_back(appo);
                }
            }
        }

        while(ss >> value && start_add == false){
            if(value == "Type:" || value == "Type" || value == "type" || value == "type:"){ss>>type;}
            if(value == "ID"){
                ss>>value;
                if(value == "R[m]"){
                    start_add = true;
                }
            }
        }
        
    }while(!in.eof());
    
    in.seekg(0);
    in.close();


    if(debug){cout<<"DG_FromFile Ends, filename = "<<Filename<<endl;}
};