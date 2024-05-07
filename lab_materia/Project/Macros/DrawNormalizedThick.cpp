#include "General.h"
#include "TDistribution.h" //not a root library
#include <fstream>

#include "TApplication.h"



int debug = 0;
int print = 0;
int fit = 0;

string path = "../Normalized_Thickness/distribution_data.txt";
string name_print;

// Function to read all TDistributions from a file into a single TDistribution object
void ReadAllTDistributionsFromFile(const string& filename, TDistribution& tdistribution) {
    if(debug){cout<<"ReadAllTDistributionsFromFile Begins"<<endl;}
    // Open the file
    ifstream file(filename);
    if (!file.is_open()) {
        cout << "Error: Could not open file " << filename << std::endl;
        return;
    }

    while (!file.eof()){
        // Start of a TDistribution section
        TDistribution td;
        td.Read(file); // Read the TDistribution from the file
        // Append all points from this TDistribution to the main TDistribution
        for (const auto& point : td.GetPoints()) {
            if(debug){cout<<"tdistribution.AddPoint(point);"<<endl;}
            tdistribution.AddPoint(point);
        }
    }

    // Close the file
    file.close();

    if(debug){cout<<"ReadAllTDistributionsFromFile Ends"<<endl;}
}


int main(int argc, const char** argv){

    TApplication myApp("myApp",0,0);
    cout<<"======================================================="<<endl;
    cout<<"Running Configuration"<<endl;
    for(int i = 1; i<argc; i++){
    
        if(!strcmp(argv[i], "-debug")){debug = atoi(argv[++i]);}
        if(!strcmp(argv[i], "-print")){print = atoi(argv[++i]);}
    }
    cout<<"debug:\t"<<debug<<endl;
    cout<<"print:\t"<<print<<endl;
    cout<<"======================================================="<<endl<<endl;
    if(debug){cout<<"Main Starts"<<endl;}

    setDefaultMinimizerOptions();

    TDistribution MyDist;
    ReadAllTDistributionsFromFile(path, MyDist);
    MyDist.DoAllGraphs(1);

    myApp.Run();

    return 0;
}