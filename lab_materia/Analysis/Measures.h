#ifndef __Measures_h__
#define __Measures_h__


#include <iostream>
#include <vector>
#include <fstream>

using namespace std;

extern int debug;
extern int print;

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

    void Print() const ;
    void Print(ofstream *out) const ;

    void Read_Data(stringstream &ss);

    void ReadAllData(const char* filename);

    double Bias_Interpolator(const double &lambda) const ;

    void Correct(const Measurements &input, const Measurements &bias);

    void SetBias(const Measurements &Meas);  

    private:
    vector<Measure> _Data;
    Measure _Calibration;
    double _Temp;
    double _Theta;
};

class TabValues{

    public:
    TabValues();
    
    Measurements GetNMeas() const ;
    Measurements GetKMeas() const ;

    void MoveMeas(const Measurements nMeas, const Measurements kMeas);

    void Print() const ;

    void ReadTabData(const char* filename);
    private:
    Measurements _nMeas;
    Measurements _kMeas;
    double _Temp;
};

#endif