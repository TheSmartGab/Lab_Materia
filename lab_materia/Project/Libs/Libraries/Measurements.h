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
        graph = new TGraphErrors();
        LorentzPeackF = new TF1();
        BackgroundF = new TF1();
        LorentzBackgroundF = new TF1();

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
        graph = new TGraphErrors(*other.graph);
        LorentzPeackF = new TF1(*other.LorentzPeackF);
        BackgroundF = new TF1(*other.BackgroundF);
        LorentzBackgroundF = new TF1(*other.LorentzBackgroundF);

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

        if(LorentzPeackF){
            delete LorentzPeackF;
            LorentzPeackF = nullptr;
        }
        if(BackgroundF){
            delete BackgroundF;
            BackgroundF = nullptr;
        }        
        if(LorentzBackgroundF){
            delete LorentzBackgroundF;
            LorentzBackgroundF = nullptr;
        }
    };

    Measurements operator-(const Measurements& other) const {
        // Create a new Measurements object to store the result
        Measurements result;

        // Check if both objects have the same number of measurements
        if (_Data.size() != other._Data.size()) {
            cout << "Error: Cannot subtract Measurements with different numbers of data points." << endl;
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

    Measurements operator/(const Measurements& other) const {
        // Create a new Measurements object to store the result
        Measurements result;

        // Check if both objects have the same number of measurements
        if (_Data.size() != other._Data.size()) {
            cout << "Error: Cannot subtract Measurements with different numbers of data points." << endl;
            return result; // Return an empty result
        }

        // Subtract corresponding measurements
        for (size_t i = 0; i < _Data.size(); ++i) {
            // Create a new Measure object to store the result of subtraction
            Measure divided_measure = _Data[i] / other._Data[i];
            // Add the result to the result's _Data vector
            result.push_back(divided_measure);
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
    
    Measurements& operator=(const Measurements& other) {
        if (this == &other) // Self-assignment check
            return *this;

        // Copy simple data members
        _Temp = other._Temp;
        _Theta = other._Theta;
        _Thick = other._Thick;
        _Rate = other._Rate;
        ID = other.ID;
        Type = other.Type;

        // Clean up existing resources
        delete func;
        delete graph;
        delete Thick_func;
        delete can;
        delete legend;

        // Allocate new resources and copy data
        func = new TF1(*other.func);
        graph = new TGraphErrors(*other.graph);
        Thick_func = new TF1(*other.Thick_func);
        can = new TCanvas();
        legend = new TLegend(*other.legend);

        LorentzBackgroundF = new TF1(*other.LorentzBackgroundF);
        BackgroundF = new TF1(*other.BackgroundF);
        LorentzPeackF = new TF1(*other.LorentzPeackF);


        // Copy vector of objects
        _Data = other._Data;
        _Calibration = other._Calibration;

        return *this;
    }


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
    string GetRun() const {return Run;}

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
                  const double& min = 0, const double& max = 1, const double& xmin = 250, const double& xmax = 1000, 
                  const double& PTSize = 0.7
                  );
    void Draw(const char* opt = "AP", const char* legend_entry = "", const double& xl1 = 0.7, const double& xl2=1, const double yl1=0.7, const double yl2=0.8, bool noErrors = true);

    void DrawCanvas(TCanvas &can, const char* opt = "P", const double& xl1 = 0.7, const double& xl2=1, const double& yl1=0.7, const double& yl2=0.8, bool noErrors = true, const char* LegendEntry="");

    void AddGraph(TGraph* graph, const char* opt){
        can->cd();
        graph->Draw();
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
    };

    double Background(double* x, double* par){

        //adjust the function to better match the background,
        //it does not look like a flat function as, for higher wl,
        //way outside the peack, it decrease without
        //any horizontal asymptote in sight
        return par[3];
    }

    double LorentzPeack(double* x, double* par){
        //par[0] -> I
        //par[1] -> gamma
        //par[2] -> x0

        double var = x[0];
        double par2 = par[2];
        return par[0]* ( (pow(par[1], 2)) / ( pow(var - par2, 2) + pow(par[1], 2) ) );
    }

    double LorentzBackground(double* x, double* par){
        //par[0] -> I
        //par[1] -> gamma
        //par[2] -> x0
        //par[3] -> background

        return LorentzPeack(x, par) * par[4] + Background(x,par);
    }


    double LorentzFit(const double& xmin = 350, const double& xmax = 700, const int& numpars=4){
        LorentzBackgroundF = new TF1("LorentzBackground", this, &Measurements::LorentzBackground, xmin, xmax, numpars);

        LorentzBackgroundF->SetParameter(0, 1);
        LorentzBackgroundF->SetParameter(1, 1);
        LorentzBackgroundF->SetParameter(2, 595);
        LorentzBackgroundF->SetParameter(3, 1);

        LorentzBackgroundF->SetParLimits(3, 0.9, 1.1);

        graph->Fit(LorentzBackgroundF, "R+");
    }

    void Log(){
        if(debug){
            cout<<"Log Begins"<<endl;
        }

        for(auto &meas : _Data){
            meas.SetValue(log(meas.GetValue()));
        }

        if(debug){
            cout<<"Log Ends"<<endl;
        }
    }

    Measure Max(){
        auto it = max_element(_Data.begin(),
                                _Data.end(),
                                [](const Measure& a,const Measure& b) { return a.GetValue() < b.GetValue(); });                 
        if (it == _Data.end()) throw "max_element called on emtpy vector";
        auto max = *it;

        return max;
    }

    int GetIndexLambda(const double &lambda){
        int count = 0;
        for(auto & measure : _Data){
            if(measure.GetLambda() == lambda){
                return count;
            }
            count ++;
        }

        cout<<"Error in GetIndexLambda: Lambda Not found, return 0. Lambda Passed: "<<lambda<<endl;
        return 0;
    }

    Measure GetMeasureLambda(const double& lambda){
        return _Data[GetIndexLambda(lambda)];
    }

    double MaxAbs(const double& lambdaMin, const double& lambdaMax){

        Sort();
        int MinIndex = GetIndexLambda(lambdaMin);
        int MaxIndex = GetIndexLambda(lambdaMax);

        auto it = max_element(_Data.begin() + MinIndex,
                        _Data.begin() + MaxIndex,
                        [](const Measure& a,const Measure& b) { return abs(a.GetValue()) < abs(b.GetValue()); });                 
        if (it == _Data.end()) throw "max_element called on emtpy vector";
        double max = (*it).GetValue();

        return max;
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
    string Run;

    TF1 *func;
    TGraphErrors *graph;
    TCanvas *can;
    TLegend *legend;

    TF1 *Thick_func;

    TF1 *LorentzPeackF;
    TF1 *BackgroundF;
    TF1 *LorentzBackgroundF;
    
};

#endif