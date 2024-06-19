#ifndef Cooked_h
#define Cooked_h


#include "General.h"
#include "Measure.h"
#include "Measurements.h"

class Cooked{

    public:

    Cooked() {
        AllCan = new TCanvas();
        DummyGraph = new TGraph();

    }
    ~Cooked() {

        if(debug){
            cout<<"==============================================="<<endl;
            cout<<"Destroing Cooked..."<<endl;
            cout<<"==============================================="<<endl;
        }
        if(AllCan){delete AllCan; AllCan = 0x0;}
        if(DummyGraph){delete DummyGraph; DummyGraph = 0x0;}
    }

    vector<Measurements> GetRunData(){return _RunData;};
    void LoadRunData();

    void ChargeDataNames();
    void CorrectAll();

    void Init(string run_num);

    void SetAll(const double& xmin = 300, const double& xmax = 900, const double& min = 0, const double& max = 1, const double& PTSize = 0.8);
    void DrawAll();

    void DrawAllSame(const char* opt = "P same", const double& min = 0, const double& max = 1, const double& xmin = 300, const double& xmax = 900, const char* XTitle = "lambda [nm]", const char* YTitle = "Transmittance");

    void AddToCanvas(TCanvas* can, int index){
        _CorrectData[index].DrawCanvas(*can, "P same");
    }

    void CloseAllCan(){
        if(AllCan != 0x0){
            AllCan->Close();
        }
    }

    void CloseRaws(){
        for(auto & raw : _RunData){
            raw.Close();
        }
    };

    void CloseCorrected(){
        for(auto & corr : _CorrectData){
            corr.Close();
        }
    };

    void UpdateAll(){
        for(auto & meas : _CorrectData){
            meas.UpdateCanvas();
        }
    }

    private:
    vector<Measurements> _RunData; //Raw Data from file
    vector<Measurements> _CorrectData; //data corrected from bias
    string Run_Num;
    vector<string> v_data_names;
    string Bias_File;
    Measurements Bias;

    string Print_Path;
    string Print_Name_Raw;
    string Print_Name_Correct;

    double xl1, xl2, yl1, yl2;

    TCanvas* AllCan;
    TGraph* DummyGraph;

    int color;
};


#endif