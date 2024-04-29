#ifndef Run_h
#define Run_h

#include "General.h"
#include "Info.h"
#include "Measure.h"
#include "Measurements.h"

class Run: public Disk_Geo, Run_Info{

    public:

    Run() : Disk_Geo(), Run_Info() {
        AllCan = new TCanvas();
        InnCan = new TCanvas();
        OutCan = new TCanvas();
        AddCan = new TCanvas();
        DummyGraph = new TGraph();
    }
    ~Run() {

        if(debug){
            cout<<"==============================================="<<endl;
            cout<<"Destroing Run..."<<endl;
            cout<<"==============================================="<<endl;
        }
        if(AllCan){delete AllCan; AllCan = 0x0;}
        if(InnCan){delete InnCan; InnCan = 0x0;}
        if(OutCan){delete OutCan; OutCan = 0x0;}
        if(AddCan){delete AddCan; AddCan = 0x0;}
        if(DummyGraph){delete DummyGraph; DummyGraph = 0x0;}
    }

    vector<Measurements> GetRunData(){return _RunData;};
    void LoadRunData();

    void ChargeDataNames();
    void CorrectAll();

    void Init(string run_num);

    void SetAll();
    void DrawAll();

    void DrawAllSame(const char* opt = "P same", const double& min = 0, const double& max = 1, const double& xmin = 300, const double& xmax = 900, const char* XTitle = "lambda [nm]", const char* YTitle = "Transmittance");
    void DrawInner(const char* opt = "P same", const double& min = 0, const double& max = 1, const double& xmin = 300, const double& xmax = 900, const char* XTitle = "lambda [nm]", const char* YTitle = "Transmittance");
    void DrawOuter(const char* opt = "P same", const double& min = 0, const double& max = 1, const double& xmin = 300, const double& xmax = 900, const char* XTitle = "lambda [nm]", const char* YTitle = "Transmittance");
    void DrawAdditional(const char* opt = "P same", const double& min = 0, const double& max = 1, const double& xmin = 300, const double& xmax = 900, const char* XTitle = "lambda [nm]", const char* YTitle = "Transmittance");

    void FitAllThick(TabValues &Tab, const double& xmin = 350, const double& xmax = 460);

    void AddToCanvas(TCanvas* can, int index){
        _CorrectData[index].DrawCanvas(*can, "P same");
    }

    void CloseALlCan(){
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
    TCanvas* InnCan;
    TCanvas* OutCan;
    TCanvas* AddCan;

    TGraph* DummyGraph;

    int color;
};

#endif