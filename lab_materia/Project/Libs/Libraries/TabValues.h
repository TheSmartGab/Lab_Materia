#ifndef TabValues_h
#define TabValues_h

#include "General.h"
#include "Measure.h"
#include "Measurements.h"

class TabValues{

    public:
    TabValues(): _kMeas(), _nMeas(), _TMeas(), _AMeas(), _RMeas(){
        can = new TCanvas(); 
        nFit = new TF1(); 
        kFit = new TF1(); 
        InterpoledT = new TGraph();
        Grade = 0;
        subrangeBoundaries = nullptr; // Adjust as needed, Be aware that it is required that the number of parameters is a multiple of number of subranges
        Size = 0;

        _Temp = 0; //kelvin
        _Thick = 0; //nm
        _Rate = 0; //nm/s
    };


    TabValues(int grade, double* arr = nullptr, int size = 0) : _kMeas(), _nMeas(), _TMeas(), _AMeas(), _RMeas() {
        can = new TCanvas(); 
        InterpoledT = new TGraph();
        Grade = grade;

        if (!arr) {
            Size = 5;
            arr = new double[Size]{280., 340. ,400., 525., 1000.};
        } else {
            Size = size;
        }

        subrangeBoundaries = arr;

        double *parameters = new double[(Grade + 1) * (Size - 1)];
        std::fill(parameters, parameters + (Grade + 1) * (Size - 1), 0.0);

        nFit = new TF1("nFit", this, &TabValues::CustomPiecewise, subrangeBoundaries[0], subrangeBoundaries[Size - 1], (Grade + 1) * (Size - 1));
        kFit = new TF1("kFit", this, &TabValues::CustomPiecewise, subrangeBoundaries[0], subrangeBoundaries[Size - 1], (Grade + 1) * (Size - 1));

        for(int i=0; i<grade+1; i++){
            nFit->SetParameter(i, 1000);
            kFit->SetParameter(i, 1000);
        }

        nFit->SetParameters(parameters);
        kFit->SetParameters(parameters); 
    };

    ~TabValues() {
        if (debug) {
            cout << "===============================================" << endl;
            cout << "Destroying TabValues..." << endl;
            cout << "===============================================" << endl;
        }
        
        // Deallocate dynamically allocated memory
        if (can != nullptr) {
            delete can;
            can = nullptr; // Set to nullptr after deletion to avoid dangling pointer
        }

        if (nFit != nullptr) {
            delete nFit;
            nFit = nullptr; // Set to nullptr after deletion to avoid dangling pointer
        }

        if (kFit != nullptr) {
            delete kFit;
            kFit = nullptr; // Set to nullptr after deletion to avoid dangling pointer
        }
        if(InterpoledT){
            delete InterpoledT;
            InterpoledT = 0x0;
        }

        // Deallocate subrangeBoundaries array
        delete[] subrangeBoundaries;
        subrangeBoundaries = nullptr;
    };

    Measurements GetNMeas() const ;
    Measurements GetKMeas() const ;

    void Print() const ;
    void TPrint() const {_TMeas.Print();};

    void SetFitN(const char* function, double x_min, double x_max, const vector<double> &params, int line_col = 2, const char* entry = "");
    void FitN(const double &xmin = 0.350, const double &xmax = 900.){_nMeas.Fit(xmin, xmax);};
    void FitNPW(){
        _nMeas.Fit(nFit, "QR+");
    }

    void SetFitK(const char* function, double x_min, double x_max, const vector<double> &params, int line_col = 3, const char* entry = "");
    void FitK(const double &xmin = 350., const double &xmax = 900.){_kMeas.Fit(xmin, xmax);};
    void FitKPW(){
        _kMeas.Fit(kFit, "QR+");
    }
    void SetFitT(const char* function, double x_min, double x_max, const vector<double> &params, int line_col = 3, const char* entry = "");
    void FitT(const double &xmin = 350., const double &xmax = 900.){_TMeas.Fit(xmin, xmax);};

    void ReadTabData(const char* filename);
    void UpdateART();

    void SetThick(const double& Thick){_Thick = Thick; _nMeas.SetThick(Thick); _kMeas.SetThick(Thick);};
    void SetTemp(const double& Temp){_Temp = Temp; _nMeas.SetTemp(Temp); _kMeas.SetTemp(Temp);};
    void SetRate(const double& Rate){_Rate = Rate; _nMeas.SetRate(Rate); _kMeas.SetRate(Rate);};

    double GetThick() const {return _Thick;};
    double GetTemp() const {return _Temp;};
    double GetRate() const {return _Rate;};

    void SetGraphN(const char* Title ="", const char* x_label="", const char* y_label="", int MS =4 , int MC=1, const double& min = 0, const double& max = 8, const double& xmin = 250, const double& xmax = 1000){
        _nMeas.SetGraph(Title, x_label, y_label, MS, MC, min, max, xmin, xmax);
    };
    void SetGraphK(const char* Title ="", const char* x_label="", const char* y_label="", int MS =4 , int MC=1, const double& min = 0, const double& max = 8, const double& xmin = 250, const double& xmax = 1000){
        _kMeas.SetGraph(Title, x_label, y_label, MS, MC, min, max, xmin, xmax);
    };

    void SetGraphA(const char* Title ="", const char* x_label="", const char* y_label="", int MS =4 , int MC=1, const double& min = 0, const double& max = 1, const double& xmin = 250, const double& xmax = 1000 ){
        _AMeas.SetGraph(Title, x_label, y_label, MS, MC, min, max, xmin, xmax);
    };    
    
    void SetGraphR(const char* Title ="", const char* x_label="", const char* y_label="", int MS =4 , int MC=1, const double& min = 0, const double& max = 1, const double& xmin = 250, const double& xmax = 1000 ){
        _RMeas.SetGraph(Title, x_label, y_label, MS, MC, min, max, xmin, xmax);
    };

    void SetGraphT(const char* Title ="", const char* x_label="", const char* y_label="", int MS =4 , int MC=1, const double& min = 0, const double& max = 0.5 , const double& xmin = 250, const double& xmax = 1000 ){
        _TMeas.SetGraph(Title, x_label, y_label, MS, MC, min, max, xmin, xmax);


    };
    
    void DrawN(const char *opt="AP", const char *LegendEntry="", const double& xl1=0.7, const double&xl2 = 1, const double& yl1 = 0.9, const double& yl2 = 1){
        _nMeas.Draw(opt, LegendEntry, xl1, xl2, yl1, yl2);
    }
    void DrawK(const char *opt="AP", const char *LegendEntry="", const double& xl1= 0.7, const double&xl2 = 1, const double& yl1 = 0.9, const double& yl2 = 1){
        _kMeas.Draw(opt, LegendEntry, xl1, xl2, yl1, yl2);
    }
    void DrawA(const char *opt="AP", const char *LegendEntry="", const double& xl1= 0.7, const double&xl2 = 1, const double& yl1 = 0.9, const double& yl2 = 1){
        _AMeas.Draw(opt, LegendEntry, xl1, xl2, yl1, yl2);
    }
    void DrawR(const char *opt="AP", const char *LegendEntry="", const double& xl1= 0.7, const double&xl2 = 1, const double& yl1 = 0.9, const double& yl2 = 1){
        _RMeas.Draw(opt, LegendEntry, xl1, xl2, yl1, yl2);
    }
    void DrawT(const char *opt="AP", const char *LegendEntry="", const double& xl1= 0.7, const double&xl2 = 1, const double& yl1 = 0.9, const double& yl2 = 1){

        int Nvalues = 10000;
        double xmin = 350; 
        double xmax = 800;

        double step = (xmax - xmin)/Nvalues;
        double lambda = xmin;

        for(int i=0; i<Nvalues; i++){
            lambda += step;
            InterpoledT->SetPoint(i, lambda, ComputeT(lambda));
        }

        InterpoledT->SetMarkerStyle(8);
        InterpoledT->SetMarkerColor(2);
        InterpoledT->SetMarkerSize(0.35);

        _TMeas.Draw(opt, LegendEntry, xl1, xl2, yl1, yl2);
        _TMeas.AddGraph(InterpoledT, "P");
        //_TMeas.Draw(opt, LegendEntry, xl1, xl2, yl1, yl2);

    }
    void DrawT(TCanvas *can, const char *opt="AP", const double& xl1= 0.7, const double&xl2 = 1, const double& yl1 = 0.9, const double& yl2 = 1){
        _TMeas.DrawCanvas(*can, opt, xl2, yl1, yl2);
    }    
    void Draw(const char* opt = "P same",const double& min=0, const double& max=8, const double& xmin=250, const double& xmax=1000){
        can->cd();
        auto frame = can->DrawFrame(xmin, min, xmax, max);
        can->Update();
        _kMeas.DrawCanvas(*can, opt, 0.8, 1., 0.8, 1.);
        _nMeas.DrawCanvas(*can, opt, 0.8, 1., 0.6, 0.8);

        if(print){
            can->cd();
            can->Print((path + "out_" + name_print + ".pdf").c_str(), (path + "out_" + name_print + ".pdf").c_str());
        }
    };

    void DrawCanvas(TCanvas &ext_can, const char*opt = "same P", const double& xl1=0.8, const double& xl2=1, const double& yl1=0.6, const double& yl2=1){
        ext_can.cd();
        _kMeas.DrawCanvas(ext_can, opt, xl1, xl2, yl1, (yl1+yl2)/2);
        _nMeas.DrawCanvas(ext_can, opt, xl1, xl2, (yl1+yl2)/2, yl2);
    };

    void DoAll(string path, string name, string expr = "", vector<double> params = vector<double>(), int ncolor = 3 , int kcolor = 4, int nfitcolor = 2, int kfitcolor = 2, const double& Tmin=0, const double& Tmax=0.2);

    double nLI(const double &lambda) const {
        return _nMeas.Linear_Interpolator(lambda);
    }
    double kLI(const double &lambda) const {
        return _kMeas.Linear_Interpolator(lambda);
    }
    double fitter(double *x, double *par){
        //linear interpolation between points
        // double n = nLI(x[0]);
        // double k = kLI(x[0]);

        //polynomial interpolation in different subranges
        double n = nFit->Eval(x[0]);
        double k = kFit->Eval(x[0]);
        double alpha = 4*M_PI*k / x[0];

        return (1 - ( (pow(n-1., 2) + pow(k,2)) / (pow(n+1., 2) + pow(k, 2)) )) * exp( -alpha * par[0]);
    }

    double ComputeT(const double& lambda){
        double n = nFit->Eval(lambda);
        double k = kFit->Eval(lambda);
        double alpha = 4*M_PI*k/lambda;

        return (1-( (pow(n-1., 2) + pow(k,2)) / (pow(n+1., 2) + pow(k, 2)) )) * exp( -alpha * GetThick());
    }

    void ScaleLambda(const double &scale){
        _nMeas.ScaleLambda(scale);
        _kMeas.ScaleLambda(scale);
    }

    void Merge(const vector<TabValues*> &Tabs);

    double CustomPiecewise(double *x, double *par) {
        // Define the subranges and their corresponding polynomial fits
        int numPar = Grade + 1;

        // Find the subrange that the input x belongs to
        int subrangeIndex = -1;
        for (int i = 0; i < Size-1; i++) {
            if (x[0] >= subrangeBoundaries[i] && x[0] <= subrangeBoundaries[i + 1]) {
                subrangeIndex = i;
                break;
            }
        }

        // If x is outside all subranges, return 0
        if (subrangeIndex == -1) {
            if(debug){cout<<"CustomPiecewise: Warning: lambda passed "<<x[0]<<" is out of range, check subrange boundaries"<<endl;}
            return 0;
        }

        // Evaluate the corresponding polynomial fit for the subrange
        double result = 0;
        int par_index = (Grade +1) * subrangeIndex;
        for (int i = 0; i <= Grade; ++i) {
            result += pow(x[0], i) * par[par_index+i]; 
        }

        return result;
    };

    void Close(){
        _nMeas.Close();
        _kMeas.Close();
        _TMeas.Close();
        _RMeas.Close();
        _AMeas.Close();
    }

    private:
    Measurements _nMeas;
    Measurements _kMeas;
    Measurements _TMeas;
    Measurements _RMeas;
    Measurements _AMeas;

    int Grade;
    double *subrangeBoundaries; // Adjust as needed, Be aware that it is required that the number of parameters is a multiple of number of subranges
    int Size;

    TGraph *InterpoledT;

    TF1 *nFit;
    TF1 *kFit;

    TCanvas *can;

    double _Temp; //kelvin
    double _Thick; //nm
    double _Rate; //nm/s

};

#endif