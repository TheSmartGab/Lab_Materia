#include "Measures.h"
#include <sstream>
#include <string>
#include "TString.h"

using namespace std;

extern int debug;
extern int print;
extern int fit;

//====================================================================================================
//implementation of Measure
Measure::Measure(){;}
Measure::Measure(const double &lambda, const double &value){_lambda = lambda; _value = value;}
Measure::~Measure(){;}

void Measure::SetLambda(const double& lambda){_lambda = lambda;}
void Measure::SetValue(const  double& value){_value = value;}

double Measure::GetLambda() const {return _lambda;}
double Measure::GetValue() const {return _value;}

void Measure::Print() const {
    cout<<_lambda<<"\t\t"<<_value<<endl;
}
void Measure::Print(ofstream *out, bool calib) const {
    if(calib){
        *out<<"lambda:\t"<<_lambda<<endl;
        *out<<"Abs:\t"<<_value<<endl;
    }
    else{*out<<_lambda<<"\t\t"<<_value<<endl;}
}

//====================================================================================================
//implementation of Measurements

void Measurements::Print() const {
    cout<<"lambda\t\tvalue"<<endl;
    cout<<"Calibration:"<<endl;
    _Calibration.Print();

    cout<<"Data:"<<endl;
    for(auto it = begin(_Data); it != end(_Data); ++it){
        it->Print();
    }
}
void Measurements::Print(ofstream *out) const {
    *out<<"lambda\t\tvalue"<<endl<<endl;
    *out<<"Calibration:"<<endl;
    _Calibration.Print(out, 1);

    *out<<endl<<"Data:"<<endl<<"#"<<endl;
    for(auto it = begin(_Data); it != end(_Data); ++it){
        it->Print(out);
    }
}

void Measurements::Read_Data(stringstream &ss){
    Measure appo;
    double lambda;
    double value;
    ss >> lambda >> value; 
    if(!ss.fail()){
        appo.SetLambda(lambda);
        appo.SetValue(value);
        _Data.push_back(appo);
    }
};

void Measurements::ReadAllData(const char* filename) {
    if (debug) {
        cout << "ReadAllData Starts, filename = " << filename << endl;
    }

    ifstream in(filename);
    if (!in) {
        cout << "\nImpossibile aprire il file \"" << filename << "\"" << endl;
        exit(-1);
    }

    bool start = false;
    bool abs_found = false;
    bool lambda_found = false;

    string str;
    while (getline(in, str)) {
        if (str.empty() || str == " " || str == "\t" || str == "\n") {
            // Skip empty lines
            continue;
        }

        stringstream ss(str);
        string value = "";
        int count = 0;

        // Process non-empty lines
        if (start) {
            Read_Data(ss);
        }

        while (ss >> value) {
            if (value == "lambda:" || value == "Lambda:" || value == "Lambda" || value == "lambda") {
                ss >> value;
                _Calibration.SetLambda(atof(value.c_str()));
                lambda_found = true;
            }
            if (value == "Abs:" || value == "Abs" || value == "abs" || value == "Abs") {
                ss >> value;
                _Calibration.SetValue(atof(value.c_str()));
                abs_found = true;
            }
            if (value == "ID:" || value == "ID" || value == "id" || value == "id:") {
                ss >> value;
                ID = value;
            }
            if (value == "#") {
                start = true;
            }
            if (value == "Type:" || value == "Type" || value == "type" || value == "type:") {
                ss >> value;
                Type = value;
            }
        }
    }

    if (start == false) {
        cout << "Error while loading " << filename << ": missing #" << endl;
        exit(-1);
    }

    if (!lambda_found) {
        cout << "WARNING: calibration lambda not found in file " << filename << ", set to 900" << endl;
        _Calibration.SetLambda(900.);
    }
    if (!abs_found) {
        cout << "WARNING: calibration abs not found in file " << filename << ", set to 0" << endl;
        _Calibration.SetValue(0.);
    }

    if (Type == "") {
        cout << "WARNING: Type not found, deducing it from id (15 : Center ; 11-14 : Inner ; 0-9 : Outer ; Any other : Additional)" << endl;
        try {
            int number = std::stoi(ID);
            if(number >= 0 && number <= 9){Type = "Outer";
            }else if(number >= 11 && number <= 14){Type = "Inner";
            }else if(number == 15){Type = "Center";
            }else{cout<<"ERROR: Numerical ID out of bounds: "<<number<<endl; exit(-1);}
        } catch(const std::invalid_argument&) {
            // Conversion failed, not a number
            Type = "Additional";
        } 
    }
    
    if (debug == 2) {
        cout << "Data Uploaded:" << endl;
        Print();
    }

    in.seekg(0);
    in.close();

    if (debug) {
        cout << "ReadAllData Ends, filename = " << filename << endl;
    }
};

double Measurements::Linear_Interpolator(const double &lambda) const {
    if(debug >= 2){cout<<"Linear Interpolator Starts"<<endl;}

    unsigned int N_data = _Data.size();
    for(int i = 0; i<N_data;i++){
        
        if(lambda == _Data[i].GetLambda()){
            if(debug>=2){cout<<"Linear Interpolator Ends"<<endl;}
            return _Data[i].GetValue();
        }
        if((lambda>_Data[i].GetLambda() && lambda<_Data[i+1].GetLambda()) || (lambda<_Data[i].GetLambda() && lambda>_Data[i+1].GetLambda())){
            double m = (_Data[i+1].GetValue() - _Data[i].GetValue())/(_Data[i+1].GetLambda() - _Data[i].GetLambda());
            if(debug>=2){cout<<"Linear Interpolator Ends"<<endl;}
            return _Data[i+1].GetValue() + m * (lambda - _Data[i+1].GetLambda());
        }
    }

    cout<<"ERROR in Linear Interpolator: Lambda passed is out of range "<<lambda<<endl; exit(-1);
    exit(-1);
};

void Measurements::Correct(const Measurements &input, const Measurements &bias){
    //NOTE: when calibrating the spectrophotometer, it must be set to A = 0 with nothing in. 
    if(debug){cout<<"Correct Begins"<<endl;}
    
    for(auto & it : input.GetData()){

        Measure correction;
        correction.SetLambda(it.GetLambda());
        correction.SetValue(it.GetValue()/bias.Linear_Interpolator(it.GetLambda()));

        _Data.push_back(correction);
    }

    _Theta = input.GetTheta();
    _Temp = input.GetTemp();
    _Thick = input.GetThick();
    _Calibration = input.GetCalibration();
    _Rate = input.GetRate();
    ID = input.GetID();
    Type = input.GetType();

    if(debug){cout<<"Correct Ends"<<endl;}
};

void Measurements::SetBias(const Measurements &Meas){
    if(debug){cout<<"SetBias Begins"<<endl;}

    if(debug==2){
        cout<<"OLD BIAS"<<endl;
        Print();
    }

    double calib_transmittance = 1-Meas.GetCalibration().GetValue();

    double delta = calib_transmittance - Linear_Interpolator(Meas.GetCalibration().GetLambda());
    for(auto & bias_meas : _Data){
        bias_meas.SetValue(bias_meas.GetValue() + delta);
    }

    if(debug==2){
        cout<<"NEW BIAS"<<endl;
        Print();
    }

    if(debug){cout<<"SetBias End"<<endl;}
};   

void Measurements::Transform(){
    if(debug){cout<<"Transform Begins"<<endl;}
    int N_data = _Data.size();
    for(int i = 0; i<N_data; i++){
        _Data[i].SetValue(1. - _Data[i].GetValue());
    }

    if(debug){cout<<"Transform Ends"<<endl;}
};


void Measurements::SetFit(const char* function, double x_min, double x_max, const vector<double> &params, int line_col, const char* entry){
    if(debug){cout<<"SetFit Begins"<<endl;}

    func = new TF1("func", function, x_min, x_max);
    func->SetLineColor(line_col);
    int N_par = params.size();
    for(int i=0; i<N_par; i++){
        func->SetParameter(i, params[i]);
    }
    legend->AddEntry(func, entry, "lp");

    if(debug){cout<<"SetFit Ends"<<endl;}
};

void Measurements::SetGraph(const char* Title, const char* x_label, const char* y_label,
                            int MS, int MC, const char* legend_entry , const double& min , const double& max, const double& xmin, const double& xmax
                            ){
    if(debug){cout<<"SetGraph Begins"<<endl;}
    
   string entry(legend_entry);
   entry += (" Thickness: " + to_string(_Thick));

   graph->SetTitle(Title); graph->GetXaxis()->SetTitle(x_label); graph->GetYaxis()->SetTitle(y_label);
   int N_Meas = GetData().size();
   for(int i = 0; i<N_Meas; i++){
    graph->SetPoint(i, GetData()[i].GetLambda(),GetData()[i].GetValue());
   }
    legend->AddEntry(graph, entry.c_str(), "P");



   graph->GetXaxis()->SetLimits(xmin, xmax);
   graph->SetMaximum(max);
   graph->SetMinimum(min);
   graph->SetMarkerSize(0.8);
   graph->SetMarkerStyle(MS);
   graph->SetMarkerColor(MC);

    if(debug){cout<<"SetGraph Ends"<<endl;}
};

void Measurements::Draw(const char* opt = "AP", const double& xl1, const double& xl2, const double yl1, const double yl2){
    if(debug){cout<<"Draw Begins"<<endl;}

    can->cd();
    graph->Draw(opt);
    legend->Draw();
    gPad->Update();
    legend->SetX1NDC(xl1);
    legend->SetX2NDC(xl2);
    legend->SetY1NDC(yl1);
    legend->SetY2NDC(yl2);
    gPad->Modified();

    if(print){
        can->Print((path + "out_" + name_print + ".pdf").c_str(), (path + "out_" + name_print + ".pdf").c_str());
    }

    if(debug){cout<<"Draw Ends"<<endl;}
}

void Measurements::DrawCanvas(TCanvas &ext_can, const char* opt, const double& xl1, const double& xl2, const double& yl1, const double& yl2){
    if(debug){cout<<"DrawCanvas Begins"<<endl;}
    ext_can.cd();
    graph->Draw(opt);
    legend->Draw();
    gPad->Update();
    legend->SetX1NDC(xl1);
    legend->SetX2NDC(xl2);
    legend->SetY1NDC(yl1);
    legend->SetY2NDC(yl2);
    gPad->Modified();

    if(print){
        ext_can.Print((path + "out_" + name_print + ".pdf").c_str(), (path + "out_" + name_print + ".pdf").c_str());
    }

    if(debug){cout<<"DrawCanvas Ends"<<endl;}
};

void Measurements::Fit_Thickness(TabValues Tab, const double& xmin, const double& xmax){
    Thick_func = new TF1("Thick_func", &Tab, &TabValues::fitter, xmin, xmax, 1);
    Thick_func->SetParameter(0,Tab.GetThick());
    Thick_func->SetParLimits(0, 5, 100);

    graph->Fit(Thick_func, "R");
};


//====================================================================================================
//implementation of TabValues


Measurements TabValues::GetNMeas() const {return _nMeas;}
Measurements TabValues::GetKMeas() const {return _kMeas;}

void TabValues::Print() const {
    cout<<"Measurements of n:"<<endl;
    _nMeas.Print();

    cout<<"Measurements of k;"<<endl;
    _kMeas.Print();
}

void TabValues::ReadTabData(const char* filename){

    if(debug){cout<<"ReadTabData Starts, filename = "<<filename<<endl;}

    ifstream in(filename);
    if(!in){cout<<"\nImpossibile aprire il file \""<<filename<<"\""<<endl; exit(-1); }

    bool start_n = false;
    bool start_k = false;

    do{
        string str;
        getline(in, str);
        stringstream ss(str);
        string value = "";
        TString Tstr = str;

        if(!Tstr.Contains("wl")){
            if(start_n && str != "" && str != "\n"){_nMeas.Read_Data(ss);}
            if(start_k && str != "" && str != "\n"){_kMeas.Read_Data(ss);}
        }

        while(ss >> value){
            if(value == "n"){start_n = true; start_k = false;}
            if(value == "k"){start_k = true; start_n = false;}
            if(value == "Thick:"){ss>>value; SetThick(atof(value.c_str()));}
            if(value == "Temp:"){ss>>value; SetTemp(atof(value.c_str()));}
            if(value == "Rate:"){ss>>value; SetRate(atof(value.c_str()));}
        }
        
    }while(!in.eof());

    if(debug == 2){
        cout<<"TabData Uploaded:"<<endl;
        Print();
    }
    
    in.seekg(0);
    in.close();

    if(debug){cout<<"ReadTabData Ends, filename = "<<filename<<endl;}
};

void TabValues::UpdateART(){
    if(debug>=2){cout<<"UpdateArt Begins"<<endl;}

    int N = _nMeas.GetData().size();
    Measure appo;
    for(int i=0; i<N; i++){
        double lambda = _nMeas.GetData()[i].GetLambda();
        double n = _nMeas.GetData()[i].GetValue();
        double k =  _kMeas.GetData()[i].GetValue();
        double R = (pow(n-1., 2) + pow(k,2)) / (pow(n+1.,2) + pow(k,2));
        double alpha = 4.*M_PI*k / lambda;   //note that it's 1/nm, thickness must be in nanometers as well
        double A = (1.-R)*(1 - exp(-alpha*_Thick));
        double T = (1-R)*exp(-alpha*_Thick);
    
        appo.SetLambda(lambda);

        //R
        appo.SetValue(R);
        _RMeas.push_back(appo);
        _RMeas.SetThick(_Thick);

        //A
        appo.SetValue(A);
        _AMeas.push_back(appo);
        _AMeas.SetThick(_Thick);

        //T
        appo.SetValue(T);
        _TMeas.push_back(appo);
        _TMeas.SetThick(_Thick);
    }

    if(debug>=2){cout<<"UpdateART Ends"<<endl;}
};

void TabValues::SetFitN(const char* function, double x_min, double x_max, const vector<double> &params, int line_col, const char * entry){
    _nMeas.SetFit(function, x_min, x_max, params, line_col, entry);
};

void TabValues::SetFitK(const char* function, double x_min, double x_max, const vector<double> &params, int line_col, const char* entry){

    _kMeas.SetFit(function, x_min, x_max, params, line_col, entry);
};

void TabValues::SetFitT(const char* function, double x_min, double x_max, const vector<double> &params, int line_col, const char* entry){
    _TMeas.SetFit(function, x_min, x_max, params, line_col, entry);
};



void TabValues::DoAll(string path, string name, string expr, vector<double> params, int ncolor , int kcolor, int nfitcolor, int kfitcolor){
    if(debug){cout<<"DoAll Begins"<<endl;}

    ReadTabData((path+name).c_str());
    _nMeas.ScaleLambda(1000);
    _kMeas.ScaleLambda(1000); //datas have lambda in micrometers, must be transformed into nanometers
    UpdateART();
    SetGraphN(("n "+name).c_str(), "Lambda", "", 8, ncolor, ("n "+name).c_str());
    SetGraphK(("k "+name).c_str(), "Lambda", "", 8, kcolor, ("k "+name).c_str());
    SetGraphA(("A "+name).c_str(), "Lambda", "", 8, kcolor, ("A "+name).c_str(), 0, 1);
    SetGraphR(("R "+name).c_str(), "Lambda", "", 8, kcolor, ("R "+name).c_str(), 0, 1);
    SetGraphT(("T "+name).c_str(), "Lambda", "", 8, kcolor, ("T "+name).c_str(), 0, 0.2);


    if(fit){
        SetFitK(expr.c_str(), 350, 900, params, kfitcolor,("k "+name+" Fit").c_str());
        SetFitN(expr.c_str(), 350, 900, params, nfitcolor,("n "+name+" Fit").c_str());
        SetFitT(expr.c_str(), 350, 900, params, nfitcolor,("T "+name+" Fit").c_str());
        FitK(400., 500.);
        FitN(400., 500.);
        FitT(400., 500.);
    }
    DrawK();
    DrawN();
    DrawA();
    DrawR();
    DrawT();
    Draw("P same", 0, 14, 150, 2000);

    int N_Points = 10000;
    for(int i=0; i<N_Points; i++){
        Measure appo;
        double lambda = 300.+i*500./N_Points;
        appo.SetLambda(lambda);
        appo.SetValue(_TMeas.Linear_Interpolator(lambda));
        _InterpoledT.push_back(appo);
    }

    _InterpoledT.SetGraph("Interpoled", "lambda", "", 9, 35, "Linear Interpolation", 0, 0.5);
    _InterpoledT.Draw();
    Interpoled_can->cd();
    auto frame = Interpoled_can->DrawFrame(300, 0., 800, 0.5);
    Interpoled_can->Update();

    _InterpoledT.DrawCanvas(*Interpoled_can, "P same", 0.8, 1, 0.8, 1);
    _TMeas.DrawCanvas(*Interpoled_can, "P same", 0.8, 1, 0.6, 0.8);

    _InterpoledT.Print();

    if(debug){cout<<"Do All Ends"<<endl;}
};


//================================================
//Implementation of class Run

void Glass_Info::Read(stringstream &ss){

    ss>>ID; ss>>Pos;
};

void Run_Info::RI_FromFile(const char* Filename){

    if(debug){cout<<"RI_FromFile Starts, filename = "<<Filename<<endl;}

    ifstream in(Filename);
    if(!in){cout<<"\nImpossibile aprire il file \""<<Filename<<"\""<<endl; exit(-1); }

    bool start_add = false;

    do{
        string str;
        getline(in, str);
        stringstream ss(str);
        string value = "";
        TString Tstr = str;
        string type = "";
        Glass_Info appo;

        if(start_add){
            appo.SetType(type);
            appo.Read(ss);
            Additional.push_back(appo);

        }

        while(ss >> value){
            if(value == "Type:"){ss>>type;}
            if(value == "ID"){
                ss>>value;
                if(value == "R[m]"){
                    start_add = true;
                }
            }
            if(value == "end" && start_add){start_add = false;}
        }
        
    }while(!in.eof());
    
    in.seekg(0);
    in.close();

    if(debug){cout<<"RI_FromFile Ends, filename = "<<Filename<<endl;}
};

void Disk_Geo::DG_FromFile(const char* Filename){

    if(debug){cout<<"DG_FromFile Starts, filename = "<<Filename<<endl;}

    ifstream in(Filename);
    if(!in){cout<<"\nImpossibile aprire il file \""<<Filename<<"\""<<endl; exit(-1); }

    bool start_add = false;

    do{
        string str;
        getline(in, str);
        stringstream ss(str);
        string value = "";
        TString Tstr = str;
        string type = "";
        Glass_Info appo;

        if(start_add){
            if(type == "Center"){
                appo.SetType(type);
                appo.Read(ss);
                Center.push_back(appo);
            }
            if(type == "Inner"){
                appo.SetType(type);
                appo.Read(ss);
                Inner.push_back(appo);
            }
            if(type == "Outer"){
                appo.SetType(type);
                appo.Read(ss);
                Outer.push_back(appo);
            }
        }

        while(ss >> value){
            if(value == "Type:"){ss>>type;}
            if(value == "ID"){
                ss>>value;
                if(value == "R[m]"){
                    start_add = true;
                }
            }
            if(value == "end" && start_add){start_add = false;}
        }
        
    }while(!in.eof());
    
    in.seekg(0);
    in.close();

    if(debug){cout<<"DG_FromFile Ends, filename = "<<Filename<<endl;}
};

#ifndef __Run_Data_Dir__
#define __Run_Data_Dir__
static string Run_Data_Dir = "../Run_Data/";
#endif

void Run::ChargeDataNames(){
    if(debug){cout<<"ChargeDataNames Begins"<<endl;}

    string Data_Names = Run_Data_Dir + Run_Num + "/Data_names.txt";

    ifstream in(Data_Names.c_str());
    if(!in){cout<<"\nImpossibile aprire il file \""<<Data_Names<<"\""<<endl; exit(-1); }

    do{
        string str;
        in>>str;
        if(str != "" && str != "\n"){v_data_names.push_back(str);}

    }while(!in.eof());
    in.seekg(0);
    in.close();

    if(debug){cout<<"ChargeDataNames Ends"<<endl;}
};

void Run::LoadRunData(){
    if(debug){cout<<"LoadRunData Begins"<<endl;}

    Bias.ReadAllData(Bias_File.c_str());

    for(auto &name_file : v_data_names){
        Measurements appo;
        string file = Run_Data_Dir + Run_Num + "/" + name_file;
        appo.ReadAllData(file.c_str());
        _RunData.push_back(appo);
    }

    if(debug){cout<<"LoadRunData Ends"<<endl;}
};

void Run::CorrectAll(){
    if(debug){cout<<"CorrectAll Begins"<<endl;}

    for(auto &meas : _RunData){
        Bias.SetBias(meas); //note that it is necessary to always set the bias on the measurements to be corrected
        Measurements appo;
        appo.Correct(meas, Bias);
        _CorrectData.push_back(appo);
    }

    if(debug){cout<<"CorrectAll Ends"<<endl;}
};

#ifndef __Disk_Geo_File__
#define __Disk_Geo_File__
static string Disk_Geo_File = "../Run_Info/General/Disk_Geo.txt";
#endif

void Run::Init(string run_num){
    if(debug){cout<<"Init begins, Run = "<<run_num<<endl;}

    Run_Num = run_num;
    Print_Path = "../" + Run_Data_Dir + Run_Num;
    Print_Name_Raw = Run_Num + "_Raw";
    Print_Name_Correct = Run_Num + "_Correct";
    string RI_File = "../Run_Info/" + Run_Num + "/" + Run_Num + ".txt";
    color = 1;

    xl1 = 0.9; xl2 = 1; yl1 = 0.9; yl2 = 1;

    AllCan = new TCanvas();
    InnCan = new TCanvas();
    OutCan = new TCanvas();
    AddCan = new TCanvas();

    Bias_File = BIAS_FILE;

    DG_FromFile(Disk_Geo_File.c_str());
    RI_FromFile(RI_File.c_str());
    ChargeDataNames();
    LoadRunData();
    CorrectAll();

    if(debug){cout<<"Init Ends, Run = "<< run_num<<endl;}

};

#ifndef __Legend_Limits__
#define __Legend_Limits__
static double XL1_min = 0.7;
static double YL1_min = 0.1;
#endif

void Run::DrawAllSame(const char* opt, const double& min = 0, const double& max = 1, const double& xmin, const double& xmax){
    if(debug){cout<<"DrawAllSame Begins"<<endl;}

    xl1 = 0.9; xl2 = 1; yl1 = 0.9; yl2 = 1;

    AllCan->cd();
    AllCan->SetTitle(Run_Num.c_str());
    gPad->SetTitle(Run_Num.c_str());

    auto frame = AllCan->DrawFrame(xmin, min, xmax, max);
    AllCan->Update();

    for(auto & meas : _CorrectData){
        meas.DrawCanvas(*AllCan, opt, xl1, xl2, yl1, yl2);
        if(xl1 > XL1_min){
            if(yl1 <= YL1_min){
                yl1 = 0.9; yl2 = 1;
                xl1 -= 0.1; xl2 -= 0.1;
            }else{
                yl1 -= 0.1; yl2 -= 0.1;
            }
        }
    }    

    if(debug){cout<<"DrawAllSame Ends"<<endl;} 
};
void Run::DrawInner(const char* opt, const double& min = 0, const double& max = 1, const double& xmin, const double& xmax){
    if(debug){cout<<"DrawInner Begins"<<endl;}

    xl1 = 0.9; xl2 = 1; yl1 = 0.9; yl2 = 1;

    InnCan->cd();
    InnCan->SetTitle("Inner");
    gPad->SetTitle("Inner");
    auto frame = InnCan->DrawFrame(xmin, min, xmax, max);
    InnCan->Update();

    for(auto & meas : _CorrectData){
        if(meas.GetType() == "Inner"){meas.DrawCanvas(*InnCan, opt, xl1, xl2, yl1, yl2);
            if(xl1 > XL1_min){
                if(yl1 <= YL1_min){
                    yl1 = 0.9; yl2 = 1;
                    xl1 -= 0.1; xl2 -= 0.1;
                }else{
                    yl1 -= 0.1; yl2 -= 0.1;
                }
            }
        }
    }   

    if(debug){cout<<"DrawInner Ends"<<endl;}
};
void Run::DrawOuter(const char* opt, const double& min = 0, const double& max = 1, const double& xmin, const double& xmax){
    if(debug){cout<<"DrawOuter Begins"<<endl;}

    xl1 = 0.9; xl2 = 1; yl1 = 0.9; yl2 = 1;

    OutCan->cd();
    OutCan->SetTitle("Outer");
    gPad->SetTitle("Outer");
    auto frame = OutCan->DrawFrame(xmin, min, xmax, max);
    OutCan->Update();

    for(auto & meas : _CorrectData){
        if(meas.GetType() == "Outer"){meas.DrawCanvas(*OutCan, opt, xl1, xl2, yl1, yl2);
             if(xl1 > XL1_min){
                if(yl1 <= YL1_min){
                    yl1 = 0.9; yl2 = 1;
                    xl1 -= 0.1; xl2 -= 0.1;
                }else{
                    yl1 -= 0.1; yl2 -= 0.1;
                }
            }
        }  
    }

    if(debug){cout<<"DrawOuter Begins"<<endl;}
};
void Run::DrawAdditional(const char* opt, const double& min = 0, const double& max = 1, const double& xmin, const double& xmax){
    if(debug){cout<<"DrawAdditional Begins"<<endl;}

    xl1 = 0.9; xl2 = 1; yl1 = 0.9; yl2 = 1;
    
    AddCan->cd();
    AddCan->SetTitle("Additional");
    gPad->SetTitle("Additional");
    auto frame = AddCan->DrawFrame(xmin, min, xmax, max);
    AddCan->Update();

    for(auto & meas : _CorrectData){
        if(meas.GetType() == "Additional"){meas.DrawCanvas(*AddCan, opt, xl1, xl2, yl1, yl2);
            if(xl1 > XL1_min){
                if(yl1 <= 0){
                    yl1 = 0.9; yl2 = 1;
                    xl1 -= 0.1; xl2 -= 0.1;
                }else{
                    yl1 -= 0.1; yl2 -= 0.1;
                }
            }
        }  
    }

    if(debug){cout<<"DrawAdditional Begins"<<endl;}
};

void Run::SetAll(){
    if(debug){cout<<"SetAll Begins"<<endl;}

    Bias.SetGraph("Bias", "lambda", "Transmittance", 8, 4, "Data", -1, 3.5, 320, 1000);
    int n_meas = _CorrectData.size();
    for(int i = 0; i<n_meas; i++){
        string title = v_data_names[i];
        while(color >= 10 && color < 30){color++;} //grey scale not really readable
        _CorrectData[i].SetGraph(title.c_str(), "lambda", "Transmittance", 8, color, _CorrectData[i].GetID().c_str(), 0, 0.5, 300, 1000);
        _RunData[i].SetGraph(("Raw_"+title).c_str(), "lambda", "Transmittance", 8, color, _CorrectData[i].GetID().c_str(), 0, 0.5, 300, 1000);
        color++;
    }

    if(debug){cout<<"SetAll Ends"<<endl;}
};
void Run::DrawAll(){
    if(debug){cout<<"DrawAll Begins"<<endl;}

    Bias.Draw("AP");
    int n_meas = _CorrectData.size();
    for(int i = 0; i<n_meas; i++){
        _CorrectData[i].Draw("AP");
        _RunData[i].Draw("AP");
        _RunData[i].Close();
    }

    if(debug){cout<<"DrawAll Ends"<<endl;}
};

void Run::FitAllThick(TabValues Tab){
    if(debug){cout<<"FitALlThick Begins"<<endl;}

    for(auto & measurements : _CorrectData){
        measurements.Fit_Thickness(Tab, 300, 800);
    }

    if(debug){cout<<"FitALlThick Ends"<<endl;}
};

