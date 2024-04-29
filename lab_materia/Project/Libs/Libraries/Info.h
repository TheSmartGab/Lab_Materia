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
    double GetPos() const {return Pos;};

    void SetType(const string &type){Type = type;};
    void SetID(const string &id){ID = id;};
    void SetPos(const double &pos){Pos = pos;};

    void Read(stringstream &ss);

    private:
    string Type, ID;
    double Pos;
};

class Run_Info{

    public:

    Run_Info(){;};
    ~Run_Info(){;};

    void RI_FromFile(const char* FileName);

    private:
    vector<Glass_Info> Additional;

};

class Disk_Geo{

    public: 

    Disk_Geo(){;};
    ~Disk_Geo(){;};

    vector<Glass_Info> GetCenter() const {return Center;};
    vector<Glass_Info> GetInner() const {return Inner;};
    vector<Glass_Info> GetOuter() const {return Outer;};

    void DG_FromFile(const char* FileName);

    private:
    vector<Glass_Info> Center;
    vector<Glass_Info> Inner;
    vector<Glass_Info> Outer;

};


#endif