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

Measurements::Measurements(){func = new TF1(); graph = new TGraph(); can = new TCanvas(); legend = new TLegend(0,0.8,0.2,1);};
Measurements::~Measurements(){;};

vector<Measure> Measurements::GetData() const {return _Data;}
Measure Measurements::GetCalibration() const {return _Calibration;}

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
    string value = "";
    ss>>value; appo.SetLambda(atof(value.c_str()));
    ss>>value; appo.SetValue(atof(value.c_str()));
    _Data.push_back(appo);
};

void Measurements::ReadAllData(const char* filename){
    if(debug){cout<<"ReadALlData Starts, filename = "<<filename<<endl;}

    ifstream in(filename);
    if(!in){cout<<"\nImpossibile aprire il file \""<<filename<<"\""<<endl; exit(-1); }

    bool start = false;
    bool abs_found = false;
    bool lambda_found = false;

    do{
        string str;
        getline(in, str);
        stringstream ss(str);
        string value = "";
        int count = 0;

        if(start && str != "" && str != "\n"){Read_Data(ss);}
        while(ss >> value){
            if(value == "lambda:"){
                ss>>value;
                _Calibration.SetLambda(atof(value.c_str()));
                lambda_found = true;
            }
            if(value == "Abs:"){
                ss>>value;
                _Calibration.SetValue(atof(value.c_str()));
                abs_found = true;
            }
            if(value == "ID:"){
                ss>>value;
                ID = value;
            }
            if(value == "#"){
                start = true;
            }
            if(value == "Type:"){
                ss>>value;
                Type = value;
            }
        }
        
    }while(!in.eof());

    if(start == false){ //txt files contains foo numbers, rather then eliminating them, add an # before the usefull datas
        cout<<"Error while loading "<<filename<<": missing #"<<endl;
        exit(-1);
    }

    if(!lambda_found){
        cout<<"WARNING: calibration lambda not found in file "<<filename<<", set to 900"<<endl;
        _Calibration.SetLambda(900.);
    }
    if(!abs_found){
        cout<<"WARNING: calibration abs not found in file "<<filename<<", set to 0"<<endl;
        _Calibration.SetValue(0.);
    }

    if(debug == 2){
        cout<<"Data Uploaded:"<<endl;
        Print();
    }
    
    in.seekg(0);
    in.close();

    if(debug){cout<<"ReadALlData Ends, filename = "<<filename<<endl;}
};

double Measurements::Linear_Interpolator(const double &lambda) const {
    if(debug >= 2){cout<<"Bias Starts"<<endl;}

    unsigned int N_data = _Data.size();
    for(int i = 0; i<N_data;i++){
        
        if(lambda == _Data[i].GetLambda()){
            if(debug>=2){cout<<"Bias Ends"<<endl;}
            return _Data[i].GetValue();
        }
        if(lambda>_Data[i].GetLambda()){
            if(i == 0){cout<<"ERROR in bias: Lambda passed is out of range: "<<lambda<<endl; exit(-1);}
            double m = (_Data[i+1].GetValue() - _Data[i].GetValue())/(_Data[i+1].GetLambda() - _Data[i].GetLambda());
            if(debug>=2){cout<<"Bias Ends"<<endl;}
            return _Data[i+1].GetValue() + m * (lambda - _Data[i+1].GetLambda());
        }
    }

    cout<<"ERROR in bias: Lambda passed is out of range "<<lambda<<endl; exit(-1);
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
    int N_data = _Data.size();
    for(int i = 0; i<N_data; i++){
        _Data[i].SetValue(1. - _Data[i].GetValue());
    }
};


void Measurements::SetFit(const char* function, double x_min, double x_max, const vector<double> &params, int line_col, const char* entry){

    func = new TF1("func", function, x_min, x_max);
    func->SetLineColor(line_col);
    int N_par = params.size();
    for(int i=0; i<N_par; i++){
        func->SetParameter(i, params[i]);
    }
    legend->AddEntry(func, entry, "lp");
};

void Measurements::SetGraph(const char* Title, const char* x_label, const char* y_label, int MS, int MC, const char* legend_entry , const double& min , const double& max, const double& xmin, const double& xmax){

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
};

void Measurements::Draw(const char* opt = "AP"){
    can->cd();
    graph->Draw(opt);
    legend->Draw();

    if(print){
        can->Print((path + "out_" + name_print + ".pdf").c_str(), (path + "out_" + name_print + ".pdf").c_str());
    }
}

void Measurements::DrawCanvas(TCanvas &ext_can, const char* opt){
    ext_can.cd();
    graph->Draw(opt);
    legend->Draw();

    if(print){
        ext_can.Print((path + "out_" + name_print + ".pdf").c_str(), (path + "out_" + name_print + ".pdf").c_str());
    }
};


//====================================================================================================
//implementation of TabValues

TabValues::TabValues(): _kMeas(), _nMeas(), _TMeas(), _AMeas(), _RMeas(){can = new TCanvas();}
TabValues::~TabValues(){;}

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
};

void TabValues::SetFitN(const char* function, double x_min, double x_max, const vector<double> &params, int line_col, const char * entry){
    _nMeas.SetFit(function, x_min, x_max, params, line_col, entry);
};

void TabValues::SetFitK(const char* function, double x_min, double x_max, const vector<double> &params, int line_col, const char* entry){
    _kMeas.SetFit(function, x_min, x_max, params, line_col, entry);
};



void TabValues::DoAll(string path, string name, string expr, vector<double> params, int ncolor , int kcolor, int nfitcolor, int kfitcolor){
    ReadTabData((path+name).c_str());
    _nMeas.ScaleLambda(1000);
    _kMeas.ScaleLambda(1000); //datas have lambda in micrometers, must be transformed into nanometers
    UpdateART();
    SetGraphN(("n "+name).c_str(), "Lambda", "", 8, ncolor, ("n "+name).c_str());
    SetGraphK(("k "+name).c_str(), "Lambda", "", 8, kcolor, ("k "+name).c_str());
    SetGraphA(("A "+name).c_str(), "Lambda", "", 8, kcolor, ("A "+name).c_str(), 0, 1);
    SetGraphR(("R "+name).c_str(), "Lambda", "", 8, kcolor, ("R "+name).c_str(), 0, 1);
    SetGraphT(("T "+name).c_str(), "Lambda", "", 8, kcolor, ("T "+name).c_str(), 0, 1);

    SetFitK(expr.c_str(), 180, 1000, params, kfitcolor,("k "+name+" Fit").c_str());
    SetFitN(expr.c_str(), 180, 1000, params, nfitcolor,("n "+name+" Fit").c_str());
    if(fit){
        FitK(180, 1000);
        FitN(180, 1000);
    }
    DrawK();
    DrawN();
    DrawA();
    DrawR();
    DrawT();
    Draw("P same", 0, 14, 150, 2000);
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
};

void Run::LoadRunData(){
    Bias.ReadAllData(Bias_File.c_str());

    for(auto &name_file : v_data_names){
        Measurements appo;
        string file = Run_Data_Dir + Run_Num + "/" + name_file;
        appo.ReadAllData(file.c_str());
        _RunData.push_back(appo);
    }
};

void Run::CorrectAll(){
    for(auto &meas : _RunData){
        Bias.SetBias(meas); //note that it is necessary to always set the bias on the measurements to be corrected
        Measurements appo; appo.Correct(meas, Bias);
        _CorrectData.push_back(appo);
    }
};

#ifndef __Disk_Geo_File__
#define __Disk_Geo_File__
static string Disk_Geo_File = "../Run_Info/General/Disk_Geo.txt";
#endif

void Run::Init(string run_num){
    Run_Num = run_num;
    Print_Path = "../" + Run_Data_Dir + Run_Num;
    Print_Name_Raw = Run_Num + "_Raw";
    Print_Name_Correct = Run_Num + "_Correct";
    string RI_File = "../Run_Info/" + Run_Num + "/" + Run_Num + ".txt";
    color = 1;

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

};

void Run::DrawAllSame(const char* opt, const double& min = 0, const double& max = 1, const double& xmin, const double& xmax){
    AllCan->cd();
    auto frame = AllCan->DrawFrame(xmin, min, xmax, max);
    AllCan->Update();

    for(auto & meas : _CorrectData){
        meas.DrawCanvas(*AllCan, opt);
    }     
};
void Run::DrawInner(const char* opt, const double& min = 0, const double& max = 1, const double& xmin, const double& xmax){
    if(debug){cout<<"DrawInner Begins"<<endl;}
    InnCan->cd();
    auto frame = InnCan->DrawFrame(xmin, min, xmax, max);
    InnCan->Update();

    for(auto & meas : _CorrectData){
        cout<<meas.GetID()<<"\t#"<<meas.GetType()<<"#"<<endl;
        if(meas.GetType() == "Inner"){meas.DrawCanvas(*InnCan, opt);}
    }   

    if(debug){cout<<"DrawInner Ends"<<endl;}
};
void Run::DrawOuter(const char* opt, const double& min = 0, const double& max = 1, const double& xmin, const double& xmax){
    if(debug){cout<<"DrawOuter Begins"<<endl;}
    OutCan->cd();
    auto frame = OutCan->DrawFrame(xmin, min, xmax, max);
    OutCan->Update();

    for(auto & meas : _CorrectData){
        if(meas.GetType() == "Outer"){meas.DrawCanvas(*OutCan, opt);}
    }   

    if(debug){cout<<"DrawOuter Begins"<<endl;}
};
void Run::DrawAdditional(const char* opt, const double& min = 0, const double& max = 1, const double& xmin, const double& xmax){
    if(debug){cout<<"DrawAdditional Begins"<<endl;}
    
    AddCan->cd();
    auto frame = AddCan->DrawFrame(xmin, min, xmax, max);
    AddCan->Update();

    for(auto & meas : _CorrectData){
        if(meas.GetType() == "Additional"){meas.DrawCanvas(*AddCan, opt);}
    } 

    if(debug){cout<<"DrawAdditional Begins"<<endl;}
};

void Run::SetAll(){
    Bias.SetGraph("Bias", "lambda", "Transmittance", 8, 4, "Data", -1, 3.5, 320, 1000);
    int n_meas = _CorrectData.size();
    for(int i = 0; i<n_meas; i++){
        string title = v_data_names[i];
        while(color >= 10 && color < 20){color++;} //grey scale not really readable
        _CorrectData[i].SetGraph(title.c_str(), "lambda", "Transmittance", 8, color, _CorrectData[i].GetID().c_str(), 0, 0.5, 320, 1000);
        _RunData[i].SetGraph(("Raw_"+title).c_str(), "lambda", "Transmittance", 8, color, _CorrectData[i].GetID().c_str(), 0, 0.5, 320, 1000);
        color++;
    }
};
void Run::DrawAll(){
    Bias.Draw("AP");
    int n_meas = _CorrectData.size();
    for(int i = 0; i<n_meas; i++){
        _CorrectData[i].Draw("AP");
        _RunData[i].Draw("AP");
    }
};