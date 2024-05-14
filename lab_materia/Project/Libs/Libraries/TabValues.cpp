#include "TabValues.h"

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
       // double A = (1.-R)*(1 - exp(-alpha*_Thick));
       // double A = (1 - exp(-alpha*_Thick));
       double A = alpha*_Thick;

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

void TabValues::DoAll(string path, string name, string expr, vector<double> params, int ncolor, int kcolor, int nfitcolor, int kfitcolor, const double& Tmin, const double& Tmax){
    if(debug){cout<<"DoAll Begins"<<endl;}

    ReadTabData((path+name).c_str());
    _nMeas.ScaleLambda(1000);
    _kMeas.ScaleLambda(1000); //datas have lambda in micrometers, must be transformed into nanometers
    UpdateART();
    SetGraphN(("n "+name).c_str(), "Lambda", "", 8, ncolor);
    SetGraphK(("k "+name).c_str(), "Lambda", "", 8, kcolor);
    SetGraphA(("A "+name).c_str(), "Lambda", "", 8, kcolor, 0, 1);
    SetGraphR(("R "+name).c_str(), "Lambda", "", 8, kcolor, 0, 1);
    SetGraphT(("T "+name).c_str(), "Lambda", "", 8, kcolor, Tmin, Tmax);


    // if(fit){ //this tries to fit with a polynomial the whole n, k and T measures
    //     SetFitK(expr.c_str(), 350, 900, params, kfitcolor,("k "+name+" Fit").c_str());
    //     SetFitN(expr.c_str(), 350, 900, params, nfitcolor,("n "+name+" Fit").c_str());
    //     SetFitT(expr.c_str(), 350, 900, params, nfitcolor,("T "+name+" Fit").c_str());
    //     FitK(400., 500.);
    //     FitN(400., 500.);
    //     FitT(400., 500.);
    // }

    FitNPW();
    FitKPW();

    DrawK();
    DrawN();
    DrawA();
    DrawR();
    DrawT();
    Draw("P same", 0, 8, 250, 1000);

    if(debug){cout<<"Do All Ends"<<endl;}
};

void TabValues::Merge(const vector<TabValues*> &Tabs){
    if(debug){cout<<"Merge Begins"<<endl;}

    //set the thickness to the weighted average of thicknesses 
    double thick = 0;
    double tot_weight = 0;

    for(TabValues* Tab : Tabs){
        double weight = Tab->GetNMeas().GetData().size();
        thick = (tot_weight*thick/(tot_weight + weight)) + (weight*Tab->GetThick()/(tot_weight + weight));
        tot_weight += weight;
        _nMeas.Add(Tab->GetNMeas());
        _kMeas.Add(Tab->GetKMeas());
    }

    _nMeas.Sort();
    _kMeas.Sort();
    UpdateART();

    if(debug){cout<<"Merge Begins"<<endl;}
};