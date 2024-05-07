#ifndef Run_h
#define Run_h

#include "General.h"
#include "Info.h"
#include "Measure.h"
#include "Measurements.h"
#include "TDistribution.h"

class Run: public Disk_Geo, public Run_Info, public TDistribution{

    public:

    Run() : Disk_Geo(), Run_Info(), TDistribution() {
        AllCan = new TCanvas();
        InnCan = new TCanvas();
        OutCan = new TCanvas();
        AddCan = new TCanvas();
        DummyGraph = new TGraph();
        ThickCan = new TCanvas();
        ThickGraph = new TGraph();
        TGFit = new TF1();
    }
    ~Run() {

        if(debug){
            cout<<"==============================================="<<endl;
            cout<<"Destroing Run..."<<endl;
            cout<<"==============================================="<<endl;
        }
        if(AllCan){delete AllCan; AllCan = 0x0;}
        if(InnCan){delete InnCan; InnCan = 0x0;}
        if(OutCan){delete OutCan; OutCan = 0x0;}
        if(AddCan){delete AddCan; AddCan = 0x0;}
        if(DummyGraph){delete DummyGraph; DummyGraph = 0x0;}
        if(ThickCan){delete ThickCan; ThickCan = 0x0;}
        if(ThickGraph){delete ThickGraph; ThickGraph = 0x0;}
        if(TGFit){delete TGFit; TGFit = 0x0;}
    }

    vector<Measurements> GetRunData(){return _RunData;};
    void LoadRunData();

    void ChargeDataNames();
    void CorrectAll();

    void Init(string run_num);

    void SetAll();
    void DrawAll();

    void DrawAllSame(const char* opt = "P same", const double& min = 0, const double& max = 1, const double& xmin = 300, const double& xmax = 900, const char* XTitle = "lambda [nm]", const char* YTitle = "Transmittance");
    void DrawInner(const char* opt = "P same", const double& min = 0, const double& max = 1, const double& xmin = 300, const double& xmax = 900, const char* XTitle = "lambda [nm]", const char* YTitle = "Transmittance");
    void DrawOuter(const char* opt = "P same", const double& min = 0, const double& max = 1, const double& xmin = 300, const double& xmax = 900, const char* XTitle = "lambda [nm]", const char* YTitle = "Transmittance");
    void DrawAdditional(const char* opt = "P same", const double& min = 0, const double& max = 1, const double& xmin = 300, const double& xmax = 900, const char* XTitle = "lambda [nm]", const char* YTitle = "Transmittance");

    void FitAllThick(TabValues &Tab, const double& xmin = 350, const double& xmax = 460);

    void AddToCanvas(TCanvas* can, int index){
        _CorrectData[index].DrawCanvas(*can, "P same");
    }

    void CloseAllCan(){
        if(AllCan != 0x0){
            AllCan->Close();
        }
    }

    void CloseInnCan(){if(InnCan){InnCan->Close();}}
    void CloseOutCan(){if(OutCan){OutCan->Close();}}
    void CloseAddCan(){if(AddCan){AddCan->Close();}}

    void CloseRaws(){
        for(auto & raw : _RunData){
            raw.Close();
        }
    };
    void CloseCorrected(){
        for(auto & corr : _CorrectData){
            corr.Close();
        }
    };

    void UpdateAll(){
        for(auto & meas : _CorrectData){
            meas.UpdateCanvas();
        }
    }

    void UpdateGeometry(){
        for(auto & glass : Center){
            glass.CoputePhi();
            glass.SetHeight(Height);
            glass.UpdateXY();
        }
        for(auto & glass : Inner){
            glass.CoputePhi();
            glass.SetHeight(Height);
            glass.UpdateXY();
        }
        for(auto & glass : Outer){
            glass.CoputePhi();
            glass.SetHeight(Height);
            glass.UpdateXY();
        }
        for(auto & glass : Additional){
            glass.CoputePhi();
            glass.SetHeight(Height);
            glass.UpdateXY();
        }
    }

    void SetThickGraph(const char* Title = "", const char* x_label="Theta [rad]", const char* y_label="Fitted Thickness [nm]", 
                  int MS = 8, int MC = 4,
                  const double& min = 0, const double& max = 35, const double& xmin = 0, const double& xmax = (M_PI / 2.)
                ){
        if(debug){cout<<"SetThickGraph Begins"<<endl;}

        double theta = 0;
        int i = 0; 
        for(auto & meas : _CorrectData){
            string mType = meas.GetType();
            if(mType == "Center"){theta = 0;}
            else if(mType == "Inner"){theta = GetThetaByID(Inner, meas.GetID());}
            else if(mType == "Outer"){theta = GetThetaByID(Outer, meas.GetID());}
            else if(mType == "Additional"){theta = GetThetaByID(Additional, meas.GetID());}

            ThickGraph->SetPoint(i, theta, meas.GetThick());
            i++;
        }

        ThickGraph->SetTitle(Title);
        ThickGraph->GetXaxis()->SetTitle(x_label);
        ThickGraph->GetYaxis()->SetTitle(y_label);
        ThickGraph->GetXaxis()->SetLimits(xmin, xmax);
        ThickGraph->SetMaximum(max);
        ThickGraph->SetMinimum(min);
        ThickGraph->SetMarkerSize(0.7);
        ThickGraph->SetMarkerStyle(MS);
        ThickGraph->SetMarkerColor(MC);

        if(debug){cout<<"SetThickGraph Ends"<<endl;}
    };

    void DrawThickGraph(const char* opt = "AP"){
        if(debug){cout<<"DrawThikcGraph Begins"<<endl;}
        ThickCan->cd();
        ThickGraph->Draw(opt);

        if(print){
            TCanvas can;
            can.cd();
            can.Print((path + "out_" + name_print + ".pdf[").c_str(), (path + "out_" + name_print + ".pdf[").c_str());
        }
        if(debug){cout<<"DrawThickGraph Ends"<<endl;}
    }

    void FitThickGraph(const char* opt = "R+", const double& xmin = 0, const double& xmax = M_PI / 2.){
        
        TGFit = new TF1("TGFit", "[0]*(cos(x))**[1]",xmin, xmax);
        ThickGraph->Fit(TGFit, opt);
    }

    void SetTD(){
        if(debug){cout<<"SetTD Begins"<<endl;}

        TVector3 pos;
        ThicknessPoint point;

        for(auto & meas : _CorrectData){
            string mType = meas.GetType();
            if(mType == "Center"){pos.SetXYZ(0., 0., 0.);}
            else if(mType == "Inner"){pos = GetPosByID(Inner, meas.GetID());}
            else if(mType == "Outer"){pos = GetPosByID(Outer, meas.GetID());}
            else if(mType == "Additional"){pos = GetPosByID(Additional, meas.GetID());}

            point.SetPos(pos);
            point.SetThick(meas.GetThick());
            point.SetError(0.001, 0.001, 0.001);
            point.SetThickError(meas.GetThickError());
            Points.push_back(point);
        }

        Run_Number = Run_Num;

        if(debug){cout<<"SetTD Ends"<<endl;}
    }



    private:
    vector<Measurements> _RunData; //Raw Data from file
    vector<Measurements> _CorrectData; //data corrected from bias
    string Run_Num;
    vector<string> v_data_names;
    string Bias_File;
    Measurements Bias;

    string Print_Path;
    string Print_Name_Raw;
    string Print_Name_Correct;

    double xl1, xl2, yl1, yl2;

    TCanvas* AllCan;
    TCanvas* InnCan;
    TCanvas* OutCan;
    TCanvas* AddCan;
    TCanvas* ThickCan;
    TGraph* ThickGraph;
    TF1*    TGFit;

    TGraph* DummyGraph;

    int color;
};

#endif