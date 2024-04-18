#ifndef __Measures_h__
#define __Measures_h__

#include <iostream>
#include <vector>
#include <fstream>
#include <cmath>
#include <math.h>
#include <filesystem>

#include "TString.h"
#include "TF1.h"
#include "TGraph.h"
#include "TCanvas.h"
#include "TAxis.h"
#include "TLegend.h"

using namespace std;

extern int debug;
extern int print;
extern int fit;
extern string path;
extern string name_print;

class Measure{
    public:
    Measure();
    Measure(const double &lambda, const double &value);
    ~Measure();

    void SetLambda(const double& lambda);
    void SetValue(const  double& value);

    double GetLambda() const;
    double GetValue() const ;

    void Print() const ;
    void Print(ofstream *out, bool calib = false) const;

    private:
    double _lambda;
    double _value;
};

class Measurements{
    public:
    Measurements();
    ~Measurements();

    vector<Measure> GetData() const ;
    Measure GetCalibration() const ;
    TF1 GetFunc() const {return *func;};

    void SetThick(const double& Thick){_Thick = Thick;};
    void SetTemp(const double& Temp){_Temp = Temp;};
    void SetTheta(const double& Theta){_Theta = Theta;};
    void SetRate(const double& Rate){_Rate = Rate;};

    double GetThick() const {return _Thick;};
    double GetTemp() const {return _Temp;};
    double GetTheta() const {return _Theta;};
    double GetRate() const {return _Rate;};
    string GetID() const {return ID;};
    string GetType() const {return Type;};

    void push_back(const Measure& meas){
        _Data.push_back(meas);
    }

    void Print() const ;
    void Print(ofstream *out) const ;

    void Read_Data(stringstream &ss);
    void ReadAllData(const char* filename);

    double Linear_Interpolator(const double &lambda) const ;
    void SetBias(const Measurements &Meas);  

    void Correct(const Measurements &input, const Measurements &bias);

    void Transform();

    void ScaleLambda(const double& factor){
        for(auto & meas : _Data){
            meas.SetLambda(meas.GetLambda() * factor);
        }
    }

    void SetFit(const char* function, double x_min, double x_max, const vector<double> &params, int line_color = 2, const char* entry = "");
    void Fit(const double &xmin = 0., const double &xmax = 1000){cout<<"Fitting "<<graph->GetTitle()<<endl; graph->Fit(func, "", "", xmin, xmax);}

    void SetGraph(const char* Title = "", const char* x_label="", const char* y_label="", int MS = 9, int MC = 1, const char* legend_entry = "", const double& min = -1, const double& max = 1, const double& xmin = 0, const double& xmax = 14);
    void Draw(const char* opt);

    void DrawCanvas(TCanvas &can, const char* opt);

    private:
    vector<Measure> _Data;
    Measure _Calibration;
    double _Temp;
    double _Theta;
    double _Thick;
    double _Rate;
    string ID;
    string Type;

    TF1 *func;
    TGraph *graph;
    TCanvas *can;
    TLegend *legend;
};

class Glass_Info{

    public:
    string GetType() const {return Type;};
    string GetID() const {return ID;};
    double GetPos() const {return Pos;};

    void SetType(const string &type){Type = type;};
    void SetID(const string &id){ID = id;};
    void SetPos(const double &pos){Pos = pos;};

    void Read(stringstream &ss);

    private:
    string Type, ID;
    double Pos;
};

class Run_Info{

    public:

    void RI_FromFile(const char* FileName);

    private:
    vector<Glass_Info> Additional;

};

class Disk_Geo{

    public: 
    vector<Glass_Info> GetCenter() const {return Center;};
    vector<Glass_Info> GetInner() const {return Inner;};
    vector<Glass_Info> GetOuter() const {return Outer;};

    void DG_FromFile(const char* FileName);

    private:
    vector<Glass_Info> Center;
    vector<Glass_Info> Inner;
    vector<Glass_Info> Outer;

};

#ifndef __Bias_File__
#define __Bias_File__
static string BIAS_FILE = "../Lab_Data/20_03_24/200324_aria_aria_1.txt";
#endif

class Run: public Disk_Geo, Run_Info{

    public:

    vector<Measurements> GetRunData(){return _RunData;};
    void LoadRunData();

    void ChargeDataNames();
    void CorrectAll();

    void Init(string run_num);

    void SetAll();
    void DrawAll();

    void DrawAllSame(const char* opt, const double& min, const double& max, const double& xmin = 300, const double& xmax = 900);
    void DrawInner(const char* opt, const double& min, const double& max, const double& xmin = 300, const double& xmax = 900);
    void DrawOuter(const char* opt, const double& min, const double& max, const double& xmin = 300, const double& xmax = 900);
    void DrawAdditional(const char* opt, const double& min, const double& max, const double& xmin = 300, const double& xmax = 900);

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

    TCanvas* AllCan;
    TCanvas* InnCan;
    TCanvas* OutCan;
    TCanvas* AddCan;

    int color;
};

class TabValues{

    public:
    TabValues();
    ~TabValues();
    
    Measurements GetNMeas() const ;
    Measurements GetKMeas() const ;

    void Print() const ;

    void SetFitN(const char* function, double x_min, double x_max, const vector<double> &params, int line_col = 2, const char* entry = "");
    void FitN(const double &xmin = 0.15, const double &xmax = 1.){_nMeas.Fit(xmin, xmax);};

    void SetFitK(const char* function, double x_min, double x_max, const vector<double> &params, int line_col = 3, const char* entry = "");
    void FitK(const double &xmin = 100., const double &xmax = 1000.){_kMeas.Fit(xmin, xmax);};

    void ReadTabData(const char* filename);
    void UpdateART();

    void SetThick(const double& Thick){_Thick = Thick; _nMeas.SetThick(Thick); _kMeas.SetThick(Thick);};
    void SetTemp(const double& Temp){_Temp = Temp; _nMeas.SetTemp(Temp); _kMeas.SetTemp(Temp);};
    void SetRate(const double& Rate){_Rate = Rate; _nMeas.SetRate(Rate); _kMeas.SetRate(Rate);};

    double GetThick() const {return _Thick;};
    double GetTemp() const {return _Temp;};
    double GetRate() const {return _Rate;};

    void SetGraphN(const char* Title ="", const char* x_label="", const char* y_label="", int MS =4 , int MC=1, const char* legend_entry = "", const double& min = 0, const double& max = 14, const double& xmin = 150, const double& xmax = 2000){
        _nMeas.SetGraph(Title, x_label, y_label, MS, MC, legend_entry, min, max, xmin, xmax);
    };
    void SetGraphK(const char* Title ="", const char* x_label="", const char* y_label="", int MS =4 , int MC=1, const char* legend_entry = "", const double& min = 0, const double& max = 14, const double& xmin = 150, const double& xmax = 2000){
        _kMeas.SetGraph(Title, x_label, y_label, MS, MC, legend_entry, min, max, xmin, xmax);
    };

    void SetGraphA(const char* Title ="", const char* x_label="", const char* y_label="", int MS =4 , int MC=1, const char* legend_entry = "", const double& min = 0, const double& max = 1, const double& xmin = 150, const double& xmax = 2000 ){
        _AMeas.SetGraph(Title, x_label, y_label, MS, MC, legend_entry, min, max, xmin, xmax);
    };    
    
    void SetGraphR(const char* Title ="", const char* x_label="", const char* y_label="", int MS =4 , int MC=1, const char* legend_entry = "", const double& min = 0, const double& max = 1, const double& xmin = 150, const double& xmax = 2000 ){
        _RMeas.SetGraph(Title, x_label, y_label, MS, MC, legend_entry, min, max, xmin, xmax);
    };

    void SetGraphT(const char* Title ="", const char* x_label="", const char* y_label="", int MS =4 , int MC=1, const char* legend_entry = "", const double& min = 0, const double& max = 1 , const double& xmin = 150, const double& xmax = 2000 ){
        _TMeas.SetGraph(Title, x_label, y_label, MS, MC, legend_entry, min, max, xmin, xmax);
    };
    
    void DrawN(const char *opt="AP"){
        _nMeas.Draw(opt);
    }
    void DrawK(const char *opt="AP"){
        _kMeas.Draw(opt);
    }
    void DrawA(const char *opt="AP"){
        _AMeas.Draw(opt);
    }
    void DrawR(const char *opt="AP"){
        _RMeas.Draw(opt);
    }
    void DrawT(const char *opt="AP"){
        _TMeas.Draw(opt);
    }
    void Draw(const char* opt, const double& min, const double& max, const double& xmin, const double& xmax){
        can->cd();
        auto frame = can->DrawFrame(xmin, min, xmax, max);
        can->Update();
        _kMeas.DrawCanvas(*can, opt);
        _nMeas.DrawCanvas(*can, opt);
    };

    void DrawCanvas(TCanvas &ext_can, const char*opt = "same P"){
        ext_can.cd();
        _kMeas.DrawCanvas(ext_can, opt);
        _nMeas.DrawCanvas(ext_can, opt);
    };

    void DoAll(string path, string name, string expr, vector<double> params, int ncolor=2 , int kcolor=3, int nfitcolor=1, int kfitcolor=1);

    private:
    Measurements _nMeas;
    Measurements _kMeas;
    Measurements _TMeas;
    Measurements _RMeas;
    Measurements _AMeas;
    TCanvas *can;

    double _Temp; //kelvin
    double _Thick; //nm
    double _Rate; //nm/s

};

#endif