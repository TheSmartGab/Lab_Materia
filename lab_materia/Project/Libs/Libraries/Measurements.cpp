#include "Measurements.h"
#include "TabValues.h"

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

    if (Type == "" ) {
        if(debug){cout << "WARNING: Type not found, deducing it from id (15 : Center ; 11-14 : Inner ; 0-9 : Outer ; Any other : Additional)" << endl;}
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
                            int MS, int MC,  const double& min , const double& max, const double& xmin, const double& xmax
                            ){
    if(debug){cout<<"SetGraph Begins"<<endl;}
    legend = new TLegend(0.7,0.9,1,1);

    graph->SetTitle(Title);
    graph->GetXaxis()->SetTitle(x_label);
    graph->GetYaxis()->SetTitle(y_label);
    int N_Meas = _Data.size();
    for(int i = 0; i<N_Meas; i++){
        if(debug >= 3){
            cout<<i<<" "<<_Data[i].GetLambda()<<" "<<_Data[i].GetValue()<<endl;
        }
        graph->SetPoint(i, _Data[i].GetLambda(),_Data[i].GetValue());
    }

    graph->GetXaxis()->SetLimits(xmin, xmax);
    graph->SetMaximum(max);
    graph->SetMinimum(min);
    graph->SetMarkerSize(0.7);
    graph->SetMarkerStyle(MS);
    graph->SetMarkerColor(MC);

    if(debug){cout<<"SetGraph Ends"<<endl;}
};

void Measurements::Draw(const char* opt, const char* legend_entry, const double& xl1, const double& xl2, const double yl1, const double yl2){
    if(debug){cout<<"Draw Begins"<<endl;}

    can->cd();
    if(debug>=3){graph->Print();}
    graph->Draw(opt);
    legend->Draw();
    legend->SetX1NDC(xl1);
    legend->SetX2NDC(xl2);
    legend->SetY1NDC(yl1);
    legend->SetY2NDC(yl2);
    legend->SetTextSize(Legend_Text_size);
    legend->AddEntry(graph, legend_entry, "P");
    gPad->Modified();
    gPad->Update();

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
    legend->SetX1NDC(xl1);
    legend->SetX2NDC(xl2);
    legend->SetY1NDC(yl1);
    legend->SetY2NDC(yl2);
    legend->SetTextSize(Legend_Text_size);
    gPad->Modified();
    gPad->Update();

    if(debug){cout<<"DrawCanvas Ends"<<endl;}
};

void Measurements::Fit_Thickness(TabValues &Tab, const double& xmin, const double& xmax){
    if(Thick_func){delete Thick_func; Thick_func = 0x0;}

    string name = "Thick_func" + ID;
    const char * nameC = name.c_str();

    Thick_func = new TF1(nameC, &Tab, &TabValues::fitter, xmin, xmax, 1);

    Thick_func->SetLineColor(2);
    //legend->AddEntry(Thick_func, "Fit", "lp"); //it is kinda useless to add this to the legend

    Thick_func->SetParameter(0,Tab.GetThick());
    Thick_func->SetParLimits(0, 0, 1000);

    graph->Fit(Thick_func, "", "", xmin, xmax);
    SetThick(Thick_func->GetParameter(0));
};