#ifndef __Measures_h__
#define __Measures_h__


#include <iostream>
#include <vector>
#include <fstream>

#include "TString.h"
#include "TF1.h"
#include "TGraph.h"
#include "TCanvas.h"
#include "TAxis.h"
#include "TLegend.h"

using namespace std;

extern int debug;
extern int print;
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
    void Print(ofstream *out) const ;

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

    void Print() const ;
    void Print(ofstream *out) const ;

    void Read_Data(stringstream &ss);
    void ReadAllData(const char* filename);

    double Bias_Interpolator(const double &lambda) const ;
    void SetBias(const Measurements &Meas);  

    void Correct(const Measurements &input, const Measurements &bias);

    //void SetGraph(TGraph &graph, int MS, int MC);

    void SetFit(const char* function, double x_min, double x_max, const vector<double> &params, int line_color = 2, const char* entry = "");
    //void Fit(TGraph& graph);
    void Fit(){graph->Fit(func);}

    void SetGraph(const char* Title , const char* x_label, const char* y_label, int MS , int MC, const char* legend_entry );
    void Draw(const char* opt);

    void DrawCanvas(TCanvas &can, const char* opt);


    private:
    vector<Measure> _Data;
    Measure _Calibration;
    double _Temp;
    double _Theta;

    TF1 *func;
    TGraph *graph;
    TCanvas *can;
    TLegend *legend;
};

class TabValues{

    public:
    TabValues();
    ~TabValues();
    
    Measurements GetNMeas() const ;
    Measurements GetKMeas() const ;

    void MoveMeas(const Measurements nMeas, const Measurements kMeas);

    void Print() const ;

    void SetFitN(const char* function, double x_min, double x_max, const vector<double> &params, int line_col = 2, const char* entry = "");
    //void FitN(TGraph& graph);
    void FitN(){_nMeas.Fit();};

    void SetFitK(const char* function, double x_min, double x_max, const vector<double> &params, int line_col = 3, const char* entry = "");
    //void FitK(TGraph& graph);
    void FitK(){_kMeas.Fit();};

    //void Fit(TGraph& graphn,TGraph& graphk){FitK(graphk); FitN(graphn);};

    void ReadTabData(const char* filename);

    //void SetGraphN(TGraph &graph, int MS, int MC);
    //void SetGraphK(TGraph &graph, int MS, int MC);

    void SetGraphN(const char* Title ="", const char* x_label="", const char* y_label="", int MS =4 , int MC=1, const char* legend_entry = "" ){
        _nMeas.SetGraph(Title, x_label, y_label, MS, MC, legend_entry);
    };
    void SetGraphK(const char* Title ="", const char* x_label="", const char* y_label="", int MS =4 , int MC=1, const char* legend_entry = "" ){
        _kMeas.SetGraph(Title, x_label, y_label, MS, MC, legend_entry);
    };

    void DrawN(const char *opt="AP"){
        _nMeas.Draw(opt);
    }
    void DrawK(const char *opt="AP"){
        _kMeas.Draw(opt);
    }   
    void Draw(const char* opt);

    void DrawCanvas(TCanvas &ext_can, const char*opt = "same P");

    private:
    Measurements _nMeas;
    Measurements _kMeas;
    TCanvas *can;
    double _Temp;
};

#endif