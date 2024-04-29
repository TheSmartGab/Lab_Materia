#include "Measurements.h"
#include <iostream>

using namespace std;

int debug = 0;
int print = 0;
int fit = 0;
string path = "";
string name_print = "";

string namein = "";
string nameout = "";

int main(int argc, const char** argv){

    cout<<"======================================================="<<endl;
    cout<<"Running Configuration"<<endl;
    for(int i = 1; i<argc; i++){
    
        if(!strcmp(argv[i], "-path")){string path_appo(argv[++i]); path = path_appo; cout<<"path: "<<path<<endl;}
        if(!strcmp(argv[i], "-namein")){string name_appo(argv[++i]); namein = name_appo + ".txt"; name_print += name_appo; }
        if(!strcmp(argv[i], "-nameout")){string name_appo(argv[++i]); nameout = name_appo + ".txt"; name_print += name_appo; }
        if(!strcmp(argv[i], "-debug")){debug = atoi(argv[++i]);}
        if(!strcmp(argv[i], "-print")){print = atoi(argv[++i]);}
        
    }
    cout<<"debug:\t"<<debug<<endl;
    cout<<"namein: "<<namein<<endl;
    cout<<"nameout: "<<nameout<<endl;
    cout<<"print:\t"<<print<<endl;
    cout<<"======================================================="<<endl<<endl;
    if(debug){cout<<"Main Starts"<<endl;}

    Measurements Data;
    Data.ReadAllData((path + namein).c_str());
    Data.Transform();

    ofstream out((path+nameout).c_str());
    Data.Print(&out);

    out.seekp(0);
    out.close();

    return 0;
}