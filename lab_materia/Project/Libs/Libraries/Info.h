#ifndef Info_h
#define Info_h

#include "General.h"
#include "Measure.h"
#include "Measurements.h"



class Glass_Info{

    public:

    Glass_Info(){;};
    ~Glass_Info(){;};

    string GetType() const {return Type;};
    string GetID() const {return ID;};
    double GetRho() const {return Rho;};
    double GetPhi() const {return Phi;};
    double GetTheta() const {return Theta;};
    TVector3 GetPos() const {return Pos;};

    void SetType(const string &type){Type = type;};
    void SetRho(const double& rho){Rho = rho;};
    void SetPhi(const double& phi){Phi = phi;};
    void SetTheta(const double& theta){Theta = theta;};
    void SetID(const string &id){ID = id;};
    void SetPos(TVector3 &pos){
        Pos.SetX(pos.X());
        Pos.SetY(pos.Y());
        Pos.SetZ(pos.Z());
    };

    void SetHeight(const double& height){
        Pos.SetZ(height);
        Theta = atan(Rho / height);
    };

    void CoputePhi(){
        if(Type == "Center"){Phi = 0;}
        if(Type == "Inner"){Phi = (stoi(ID) - 11) * M_PI / 2;}
        if(Type == "Outer"){Phi = stoi(ID) * M_PI / 5.;}
        if(Type == "Additional"){
            if(ID == "A"){
                Phi = 0;
            }else if(ID == "B"){
                Phi = M_PI / 2;
            }else if(ID == "C"){
                Phi = M_PI;
            }else if(ID == "D"){
                Phi = 3 * M_PI / 2;
            }
        }

    }

    void UpdateXY(){
        Pos.SetX(Rho * cos(Phi));
        Pos.SetY(Rho * sin(Phi));
    }

    void Read(stringstream &ss);

    private:
    string Type, ID;
    double Rho, Phi, Theta;
    TVector3 Pos;
};

class Run_Info{

    public:

    Run_Info(){;};
    ~Run_Info(){;};

    void RI_FromFile(const char* FileName);

    protected:
    vector<Glass_Info> Additional;
    double Height;

};

class Disk_Geo{

    public: 

    Disk_Geo(){;};
    ~Disk_Geo(){;};

    vector<Glass_Info> GetCenter() const {return Center;};
    vector<Glass_Info> GetInner() const {return Inner;};
    vector<Glass_Info> GetOuter() const {return Outer;};

    void DG_FromFile(const char* FileName);

    protected:
    vector<Glass_Info> Center;
    vector<Glass_Info> Inner;
    vector<Glass_Info> Outer;

};

inline double GetRhoByID(const vector<Glass_Info>& glassInfos, const string& id) {
    if(debug){cout<<"GetRhoByID Begins"<<endl;}
    for (const auto& glassInfo : glassInfos) {
        if (glassInfo.GetID() == id) {
            if(debug){cout<<"ID Found, return "<<glassInfo.GetRho();}
            return glassInfo.GetRho();
        }
    }
    // If ID is not found, return some default value or throw an exception
    // In this example, I'll return -1 to indicate the ID was not found
    if(debug){cout<<"ERROR in GetRhoByID: ID not found, return -9999"<<endl;}
    return -9999;
}

inline double GetThetaByID(const vector<Glass_Info>& glassInfos, const string& id) {
    if(debug){cout<<"GetThetaByID Begins"<<endl;}
    for (const auto& glassInfo : glassInfos) {
        if (glassInfo.GetID() == id) {
            if(debug){cout<<"ID Found, return "<<glassInfo.GetTheta()<<endl;}
            return glassInfo.GetTheta();
        }
    }
    // If ID is not found, return some default value or throw an exception
    // In this example, I'll return -1 to indicate the ID was not found
    if(debug){cout<<"ERROR in GetThetaByID: ID not found, return -9999"<<endl;}
    return -9999;
}

inline double GetPhiByID(const vector<Glass_Info>& glassInfos, const string& id) {
    if(debug){cout<<"GetThetaByID Begins"<<endl;}
    for (const auto& glassInfo : glassInfos) {
        if (glassInfo.GetID() == id) {
            if(debug){cout<<"ID Found, return "<<glassInfo.GetPhi()<<endl;}
            return glassInfo.GetPhi();
        }
    }
    // If ID is not found, return some default value or throw an exception
    // In this example, I'll return -1 to indicate the ID was not found
    if(debug){cout<<"ERROR in GetThetaByID: ID not found, return -9999"<<endl;}
    return -9999;
}

inline TVector3 GetPosByID(const vector<Glass_Info>& glassInfos, const string& id) {
    if(debug){cout<<"GetPosByID Begins"<<endl;}
    for (const auto& glassInfo : glassInfos) {
        if (glassInfo.GetID() == id) {
            if(debug){cout<<"ID Found, return "; glassInfo.GetPos().Print();cout<<endl;}
            return glassInfo.GetPos();
        }
    }
    // If ID is not found, return some default value or throw an exception
    // In this example, I'll return -1 to indicate the ID was not found
    if(debug){cout<<"ERROR in GetThetaByID: ID not found, return (-9999, -9999, -9999)"<<endl;}
    TVector3 error(-9999, -9999, -9999);
    return error;
}


#endif