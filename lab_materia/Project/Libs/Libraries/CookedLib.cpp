#include "Cooked.h"

void Cooked::ChargeDataNames(){
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

void Cooked::LoadRunData(){
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

void Cooked::CorrectAll(){
    if(debug){cout<<"CorrectAll Begins"<<endl;}

    for(auto &meas : _RunData){
        Bias.SetBias(meas); //note that it is necessary to always set the bias on the measurements to be corrected
        Measurements appo;
        appo.Correct(meas, Bias);
        _CorrectData.push_back(appo);
    }

    if(debug){cout<<"CorrectAll Ends"<<endl;}
};


void Cooked::Init(string run_num){
    if(debug){cout<<"Init begins, Run = "<<run_num<<endl;}

    Run_Num = run_num;
    Print_Path = "../" + Run_Data_Dir + Run_Num;
    Print_Name_Raw = Run_Num + "_Raw";
    Print_Name_Correct = Run_Num + "_Correct";
    color = 1;

    xl1 = 0.9; xl2 = 1; yl1 = 0.9; yl2 = 1;

    Bias_File = BIAS_FILE;

    ChargeDataNames();
    LoadRunData();
    CorrectAll();

    if(debug){cout<<"Init Ends, Run = "<< run_num<<endl;}

};

void Cooked::DrawAllSame(const char* opt, const double& min, const double& max, const double& xmin, const double& xmax, const char* XTitle, const char* YTitle){
    if(debug){cout<<"DrawAllSame Begins"<<endl;}

    ResetLegend(xl1, xl2, yl1, yl2);
    AllCan->cd();
    AllCan->SetTitle(Run_Num.c_str());
    gPad->SetTitle(Run_Num.c_str());

    DummyGraph->GetXaxis()->SetTitle(XTitle);
    DummyGraph->GetYaxis()->SetTitle(YTitle);
    DummyGraph->GetXaxis()->SetLimits(xmin, xmax);
    DummyGraph->GetYaxis()->SetRangeUser(min, max);
    DummyGraph->Draw("AP same");

    AllCan->Update();

    int N_Data = _CorrectData.size();
    for(int i=0; i<N_Data; i++){
        string LegendStr = _CorrectData[i].GetID() + " " + _CorrectData[i].GetRun() + "" + to_string(_CorrectData[i].GetTemp());
        _CorrectData[i].DrawCanvas(*AllCan, opt, xl1, xl2, yl1, yl2, true, LegendStr.c_str());
        UpdateLegendLimits(xl1, xl2, yl1, yl2);
    }

    if(print){
        AllCan->cd();
        AllCan->Print((path + "out_" + name_print + ".pdf").c_str(), (path + "out_" + name_print + ".pdf").c_str());
    }    

    if(debug){cout<<"DrawAllSame Ends"<<endl;} 
};

void Cooked::SetAll(const double& xmin, const double& xmax, const double& min, const double& max, const double& PTSize){
    if(debug){cout<<"SetAll Begins"<<endl;}

    Bias.SetGraph("Bias", "lambda [nm]", "Transmittance [a.u.]", 8, 4, -1, 3.5, 320, 1000, PTSize);
    int n_meas = _CorrectData.size();
    for(int i = 0; i<n_meas; i++){
        string title = v_data_names[i];
        if(color >= 10 && color < 30){color+=4;} //grey scale not really readable
        _CorrectData[i].SetGraph(title.c_str(), "lambda[nm]", "Transmittance", 8, color, min, max, xmin, xmax, PTSize);
        _RunData[i].SetGraph(("Raw_"+title).c_str(), "lambda[nm]", "Transmittance", 8, color, min, max, xmin, xmax);
        color++;
    }

    DummyGraph->SetPoint(0, -9999, -9999); //the dummy graph must have at least one point, this ios outside the range drawn

    if(debug){cout<<"SetAll Ends"<<endl;}
};

void Cooked::DrawAll(){
    if(debug){cout<<"DrawAll Begins"<<endl;}

    Bias.Draw("AP");
    int n_meas = _CorrectData.size();
    for(int i = 0; i<n_meas; i++){
        _CorrectData[i].Draw("AP", (_CorrectData[i].GetID() +": "+ to_string( _CorrectData[i].GetThick())).c_str(), 0.7, 1, 0.7, 0.8, false);
        _RunData[i].Draw("AP", (_CorrectData[i].GetID() +": "+ to_string( _CorrectData[i].GetThick())).c_str(), 0.7, 1, 0.7, 0.8, false); //Avoid Drawing Raw graphs
    }

    if(debug){cout<<"DrawAll Ends"<<endl;}
};
