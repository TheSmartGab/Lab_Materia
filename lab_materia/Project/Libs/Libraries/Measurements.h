#ifndef Measurements_h
#define Measurements_h

#include "General.h"
#include "Measure.h"

class Measurements{
    public:
    Measurements(){
        can = new TCanvas();
        Thick_func = new TF1();
        func = new TF1();
        legend = new TLegend(0.8, 0.8, 1, 1);
        graph = new TGraph();
    };

    Measurements(const Measurements &other){
        // Copy simple data members
        _Temp = other._Temp;
        _Theta = other._Theta;
        _Thick = other._Thick;
        _Rate = other._Rate;
        ID = other.ID;
        Type = other.Type;

        // Initialize new TCanvas objects (assuming they can't be copied)
        can = new TCanvas();
        Thick_func = new TF1(*other.Thick_func);
        func = new TF1(*other.func);
        legend = new TLegend(*other.legend);
        graph = new TGraph(*other.graph);

        // Copy vector of objects
        _Data = other._Data;
        _Calibration = other._Calibration;
    }
    ~Measurements(){
        if (func) {
            delete func;
            func = nullptr;
        }
        if (graph) {
            delete graph;
            graph = nullptr;
        }
        if (Thick_func) {
            delete Thick_func;
            Thick_func = nullptr;
        }
        if (can) {
            delete can;
            can = nullptr;
        }
        if (legend) {
            delete legend;
            legend = nullptr;
        }
    };

    Measurements operator-(const Measurements& other) const {
        // Create a new Measurements object to store the result
        Measurements result;

        // Check if both objects have the same number of measurements
        if (_Data.size() != other._Data.size()) {
            cerr << "Error: Cannot subtract Measurements with different numbers of data points." << endl;
            return result; // Return an empty result
        }

        // Subtract corresponding measurements
        for (size_t i = 0; i < _Data.size(); ++i) {
            // Create a new Measure object to store the result of subtraction
            Measure subtracted_measure = _Data[i] - other._Data[i];
            // Add the result to the result's _Data vector
            result.push_back(subtracted_measure);
        }

        // Copy other necessary data members from the current object
        result.SetThick(_Thick);
        result.SetTemp(_Temp);
        result.SetTheta(_Theta);
        result.SetRate(_Rate);
        result.SetID(ID);
        result.SetType(Type);

        return result;
    };

    vector<Measure> GetData() const {return _Data;}
    Measure GetCalibration() const {return _Calibration;}
    TF1 GetFunc() const {return *func;};
    TF1 GetThickFunc() const {return *Thick_func;};

    void SetThick(const double& Thick){_Thick = Thick;};
    void SetTemp(const double& Temp){_Temp = Temp;};
    void SetTheta(const double& Theta){_Theta = Theta;};
    void SetRate(const double& Rate){_Rate = Rate;};
    void SetID(const string & id){ID = id;};
    void SetType(const string & type){Type = type;};

    double GetThick() const {return _Thick;};
    double GetTemp() const {return _Temp;};
    double GetTheta() const {return _Theta;};
    double GetRate() const {return _Rate;};
    string GetID() const {return ID;};
    string GetType() const {return Type;};
    double GetThickError() const {return Thick_func->GetParError(0);}

    void push_back(const Measure &meas){
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
    void Fit(const double &xmin = 0., const double &xmax = 1000, const char* opt = ""){
        if(debug){cout<<"Fitting "<<graph->GetTitle()<<endl;}
        graph->Fit(func, opt, "", xmin, xmax);
    }

    void Fit(TF1* func, const char* opt = "R+"){
        if(debug){cout<<"Fitting "<<graph->GetTitle()<<endl;}
        graph->Fit(func, opt);
    }

    void SetGraph(const char* Title = "", const char* x_label="", const char* y_label="", 
                  int MS = 9, int MC = 1,
                  const double& min = 0, const double& max = 1, const double& xmin = 250, const double& xmax = 1000
                  );
    void Draw(const char* opt = "AP", const char* legend_entry = "", const double& xl1 = 0.7, const double& xl2=1, const double yl1=0.7, const double yl2=0.8);

    void DrawCanvas(TCanvas &can, const char* opt, const double& xl1 = 0.7, const double& xl2=1, const double& yl1=0.7, const double& yl2=0.8);
    void AddGraph(TGraph* graph, const char* opt){
        can->cd();
        graph->Draw(opt);
    }

    void Close(){
        if(debug){cout<<"Closing Canvas"<<endl;}
        if(can){can->Close(); delete can; can = 0x0;}
    }

    void Fit_Thickness(TabValues &Tab, const double& xmin, const double& xmax);

    void Sort(){
        std::sort(_Data.begin(), _Data.end(),[](const Measure& a, const Measure& b){return a.GetLambda() < b.GetLambda();});
        _Data.erase(std::unique(_Data.begin(), _Data.end(),
                   [](const Measure& a, const Measure& b) {
                       return a.GetLambda() == b.GetLambda();
                   }), _Data.end());    
    }

    void Add(const Measurements& ToAdd){
        for(auto & meas : ToAdd.GetData()){
            push_back(meas);
        }
    };

    void UpdateCanvas(){
        if(can){
            can->cd();
            gPad->Modified();
            gPad->Update();
        }
    }

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

    TF1 *Thick_func;
    
};

#endif