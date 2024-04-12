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

    void Print() const ;
    void Print(ofstream *out) const ;

    void Read_Data(stringstream &ss);
    void ReadAllData(const char* filename);

    double Bias_Interpolator(const double &lambda) const ;
    void SetBias(const Measurements &Meas);  

    void Correct(const Measurements &input, const Measurements &bias);

    void Transform();

    void SetFit(const char* function, double x_min, double x_max, const vector<double> &params, int line_color = 2, const char* entry = "");
    void Fit(const double &xmin = 0.15, const double &xmax = 1.){cout<<"Fitting "<<graph->GetTitle()<<endl; graph->Fit(func, "", "", xmin, xmax);}

    void SetGraph(const char* Title = "", const char* x_label="", const char* y_label="", int MS = 9, int MC = 1, const char* legend_entry = "", const double& min = 0, const double& max = 20 );
    void Draw(const char* opt);

    void DrawCanvas(TCanvas &can, const char* opt);


    private:
    vector<Measure> _Data;
    Measure _Calibration;
    double _Temp;
    double _Theta;
    double _Thick;
    double _Rate;

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

    void Print() const ;

    void SetFitN(const char* function, double x_min, double x_max, const vector<double> &params, int line_col = 2, const char* entry = "");
    void FitN(const double &xmin = 0.15, const double &xmax = 1.){_nMeas.Fit(xmin, xmax);};

    void SetFitK(const char* function, double x_min, double x_max, const vector<double> &params, int line_col = 3, const char* entry = "");
    void FitK(const double &xmin = 0.15, const double &xmax = 1.){_kMeas.Fit(xmin, xmax);};

    void ReadTabData(const char* filename);

    void SetThick(const double& Thick){_Thick = Thick; _nMeas.SetThick(Thick); _kMeas.SetThick(Thick);};
    void SetTemp(const double& Temp){_Temp = Temp; _nMeas.SetTemp(Temp); _kMeas.SetTemp(Temp);};
    void SetRate(const double& Rate){_Rate = Rate; _nMeas.SetRate(Rate); _kMeas.SetRate(Rate);};

    double GetThick() const {return _Thick;};
    double GetTemp() const {return _Temp;};
    double GetRate() const {return _Rate;};

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

    void DoAll(string path, string name, string expr, vector<double> params, int ncolor=2 , int kcolor=3, int nfitcolor=1, int kfitcolor=1);

    private:
    Measurements _nMeas;
    Measurements _kMeas;
    TCanvas *can;

    double _Temp; //kelvin
    double _Thick; //nm
    double _Rate; //nm/s

};

#endif