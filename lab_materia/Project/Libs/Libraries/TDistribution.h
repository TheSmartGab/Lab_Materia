#ifndef TDistribution_h
#define TDistribution_h

#include "General.h"
#include "TVector3.h"
#include "TCanvas.h"
#include "TGraphErrors.h"
#include "TF1.h"
#include "TF2.h"
#include "TH2D.h"
#include "TGraph2DErrors.h"
#include "TAxis.h"
#include <vector>
#include <fstream>

class ThicknessPoint{
public:

    ThicknessPoint() 
        : Pos(0, 0, 0), Error(0, 0, 0), Thick(0), ThickError(0) {
        // Default constructor initializes Pos, Error to (0,0,0) and Thick, ThickError to 0
    }
    ThicknessPoint(const ThicknessPoint& other) {
        Pos = other.Pos;
        Error = other.Error;
        Thick = other.Thick;
        ThickError = other.ThickError;
    }

    void SetPos(const double& x, const double& y, const double& z){
        Pos.SetX(x);
        Pos.SetY(y);
        Pos.SetZ(z);
    }

    void SetPos(const TVector3& pos){
        Pos.SetXYZ(pos.X(), pos.Y(), pos.Z());
    }

    void SetError(const double& errx, const double erry, const double errz){
        Error.SetX(errx);
        Error.SetY(erry);
        Error.SetZ(errz);
    }

    void SetThick(const double& thick){
        Thick = thick;
    }

    void SetThickError(const double err){
        ThickError = err;
    }


    TVector3 GetPos() const {
        return Pos;
    }

    TVector3 GetError() const {
        return Error;
    }

    double GetThick() const {
        return Thick;
    }

    double GetThickError() const {
        return ThickError;
    }

    void Print(){
        cout<<"Pos:"<<endl;
        Pos.Print(); 
        Error.Print();

        cout<<"Thick: "<<Thick<<" "<<ThickError<<endl;
    }

    void Print(ofstream& out){
        out<<"Point"<<endl;
        out<<"Pos: "<<Pos.X()<<" "<<Pos.Y()<<" "<<Pos.Z()<<endl;
        out<<"PosErr: "<<Error.X()<<" "<<Error.Y()<<" "<<Error.Z()<<endl;
        out<<"Thickness: "<<Thick<<endl;
        out<<"ThickError: "<<ThickError<<endl;
    }

    void Read(ifstream& in){
        if(debug){cout<<"Reading Point"<<endl;}
        string appo = "";
        in>>appo; 
        in>>appo; Pos.SetX(stod(appo));
        in>>appo; Pos.SetY(stod(appo));
        in>>appo; Pos.SetZ(stod(appo));
        in>>appo;
        in>>appo; Error.SetX(stod(appo));
        in>>appo; Error.SetY(stod(appo));
        in>>appo; Error.SetZ(stod(appo));
        in>>appo;
        in>>appo; Thick = stod(appo);
        in>>appo;
        in>>appo; ThickError = stod(appo);

        if(debug){Print();}
    }


private:
    TVector3 Pos, Error;
    double Thick, ThickError;
};


class TDistribution{

    public:

    TDistribution(){
        graph1D = new TGraphErrors();
        graph2D = new TGraph2DErrors();
        can1D = new TCanvas();
        can2D = new TCanvas();
        Fit1D = new TF1();
        Fit2D = new TF2();
        legend1D = new TLegend(0.6, 0.8, 1, 1);
        legend2D = new TLegend(0, 0.8, 1, 0.95);
        legend1D->SetTextSize(Legend_Text_size);
        legend2D->SetTextSize(Legend_Text_size);
    }

    void SetNameFitter(const string& nameFitter){name_fitter = nameFitter;}

    void DoAllGraphs(const double& initialthickness = 30, const char* Title1 = "Fitted Thickness", const char* Title2 = "Fitted Thickness",
                 const char* x_label1="Theta [rad]", const char* y_label1="Fitted Thickness [nm]", 
                 const char* x_label2="x [m]", const char* y_label2="y [m]", const char* z_label2="Fitted Thickness [nm]",
                 int MS = 8, int MC = 4,
                 const double& xmin1 = 0, const double& xmax1 = (M_PI / 2.), 
                 const double& xmin2 = -0.1, const double& xmax2 = 0.1, const double& ymin2 = -0.1, const double& ymax2 = 0.1)
    {
        if(debug){cout<<"DoAllGraphs Begins"<<endl;}
        double min = 0;
        double max = 0;

        int i = 0;
        for(auto & Point : Points){
            double thick = Point.GetThick();
            if(thick >= max){max = thick;}
            double theta = Point.GetPos().Theta();
            double x = Point.GetPos().X();
            double y = Point.GetPos().Y();
            graph1D->SetPoint(i, theta, thick);
            graph2D->SetPoint(i, x, y, thick);
            graph1D->SetPointError(i, 0, Point.GetThickError());
            graph2D->SetPointError(i, Point.GetError().X(), Point.GetError().Y(), Point.GetThickError());
            i++;
        }

        if(debug){cout<<"Graphs Set"<<endl;}

        double height = Points[0].GetPos().Z();

        graph1D->SetTitle(Title1);
        graph1D->GetXaxis()->SetTitle(x_label1);
        graph1D->GetYaxis()->SetTitle(y_label1);
        graph1D->SetMarkerStyle(MS);
        graph1D->SetMarkerColor(MC);
        graph1D->SetMinimum(min);
        graph1D->SetMaximum(1.075*max);
        graph1D->GetXaxis()->SetLimits(xmin1, xmax1);

        can1D->cd();
        graph1D->Draw("AP");

        if(debug){cout<<"Graph1D Drawn"<<endl;}

        Fit1D = new TF1("fit1D", "[0]*(cos(x))**[1]", xmin1, xmax1);
        graph1D->Fit(Fit1D, "R");

        // Get the formula of the fit and insert fitted parameters
        std::string formula1D = Fit1D->GetExpFormula().Data();
        for (int i = 0; i < Fit1D->GetNpar(); ++i) {
            std::string paramStr = Form("[p%d]", i);
            std::string paramValueStr = Form("%.2f", Fit1D->GetParameter(i));
            while (formula1D.find(paramStr) != std::string::npos) {
                formula1D.replace(formula1D.find(paramStr), paramStr.length(), paramValueStr);
            }
        }
        legend1D->AddEntry(Fit1D, formula1D.c_str(), "L");
        legend1D->AddEntry(graph1D, ("Thickness fitted with "+name_fitter).c_str(), "P");
        legend1D->Draw();

        if(debug){cout<<"Graph1D Fitted"<<endl;}

        graph2D->SetTitle(Title2);
        graph2D->GetXaxis()->SetTitle(x_label2);
        graph2D->GetYaxis()->SetTitle(y_label2);
        graph2D->GetZaxis()->SetTitle(z_label2);
        graph2D->SetMarkerStyle(MS);
        graph2D->SetMarkerColor(MC);
        graph2D->SetMinimum(min);
        graph2D->SetMaximum(1.105*max);
        graph2D->GetXaxis()->SetLimits(xmin2, xmax2);
        graph2D->GetYaxis()->SetLimits(ymin2, ymax2);

        can2D->cd();

        Fit2D = new TF2("fit2D", 
                 "[0] * TMath::Power( TMath::Cos( TMath::ATan( TMath::Sqrt( TMath::Power( x - [2], 2) + TMath::Power( y - [3], 2) ) / [4] ) ), [1] )", 
                 xmin2, xmax2, ymin2, ymax2);
        
        Fit2D->SetParameter(4, height);
        Fit2D->SetParameter(1, 4);
        Fit2D->SetParameter(2, 0);
        Fit2D->SetParameter(3, 0);
        Fit2D->SetParameter(0, initialthickness);

        Fit2D->FixParameter(4, height);
        Fit2D->SetParLimits(0, 0.9*max, 1.1*max);

        graph2D->Fit(Fit2D, "R");
        // Get the formula of the fit and insert fitted parameters
        std::string formula2D = Fit2D->GetExpFormula().Data();
        for (int i = 0; i < Fit2D->GetNpar(); ++i) {
            std::string paramStr = Form("[p%d]", i);
            std::string paramValueStr = Form("%.2f", Fit2D->GetParameter(i));
            while (formula2D.find(paramStr) != std::string::npos) {
                formula2D.replace(formula2D.find(paramStr), paramStr.length(), paramValueStr);
            }
            string TMathstr = "TMath::";
            while (formula2D.find(TMathstr) != std::string::npos) {
                formula2D.replace(formula2D.find(TMathstr), TMathstr.length(), "");
            }
        }

        graph2D->Draw("P");
        Fit2D->Draw("SURF same");
        legend2D->AddEntry(Fit2D, formula2D.c_str(), "L");
        legend2D->AddEntry(graph1D, ("Thickness fitted with "+name_fitter).c_str(), "P");
        legend2D->Draw("same");
        if(debug){graph2D->Print();}

        if(debug){cout<<"DoAllGraphs Ends"<<endl;}
    }


    void Print(){
        cout<<"Points:"<<endl;
        for(auto & point : Points){
            point.Print();
        }
        cout<<"end"<<endl;
    }

    void Print(ofstream& out){
        out<<"=================================="<<endl;
        out<<"TD_Starts"<<endl;
        out<<"RunNumber: "<<Run_Number<<endl;
        out<<"NameFitter: "<<name_fitter<<endl;
        for(auto & point : Points){
            point.Print(out);
        }
        out<<"TD_Ends"<<endl;
        out<<"=================================="<<endl;
    }

    void Read(ifstream& in){
        if(debug){cout<<"Read Begins"<<endl;}
        string appo;
        ThicknessPoint TPappo;
        do{in>>appo;}while(appo != "TD_Starts");

        in>>appo;
        in>>appo; Run_Number = appo;
        in>>appo;
        in>>appo; name_fitter = appo;
        in>>appo;
        while(appo == "Point"){
            TPappo.Read(in);
            AddPoint(TPappo);
            in>>appo;
        }
        in>>appo;

        if(debug){cout<<"Read Ends"<<endl;}
    }

    void NormalizeThickness() {
        // Find the point with the highest thickness value
        auto maxThickPoint = std::max_element(Points.begin(), Points.end(), 
            [](const ThicknessPoint& p1, const ThicknessPoint& p2) {
                return std::fabs(p1.GetThick()) < std::fabs(p2.GetThick());
            });

        // Get the maximum thickness value
        double maxThickness = maxThickPoint->GetThick();

        // Scale the thickness values and errors
        for (auto& point : Points) {
            double thickness = point.GetThick();
            double thickError = point.GetThickError();

            // Normalize thickness value
            thickness /= maxThickness;

            // Scale thickness error
            thickError /= std::fabs(maxThickness);

            // Update thickness and thickness error
            point.SetThick(thickness);
            point.SetThickError(thickError);
        }
    }

    vector<ThicknessPoint> GetPoints(){
        return Points;
    }

    void AddPoint(const ThicknessPoint& point) {
        Points.push_back(point);
    }

    protected:
    vector<ThicknessPoint> Points;

    TGraphErrors* graph1D;
    TCanvas* can1D;
    TF1* Fit1D;
    TLegend* legend1D;

    TGraph2DErrors* graph2D;
    TCanvas* can2D;
    TF2* Fit2D;
    TLegend* legend2D;

    string name_fitter;
    string Run_Number;
};

#endif