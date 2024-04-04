#ifndef __Measures_h__
#define __Measures_h__


#include <iostream>
#include <vector>

using namespace std;

#ifndef __Debug__
#define __Debug__
static int debug = 0;
#endif

#ifndef __Print__
#define __Print__
static int print = 0;
#endif

#ifndef __Files__
#define __Files__
static string path;
static string name;
static string name_print;
#endif

struct Measure{
    vector<double> _dati;

    Measure operator-(const Measure& Meas_2) const {
        if(debug){cout<<"Calling operator -"<<endl;}
        if(_dati.size() != 2 || Meas_2._dati.size()!=2){
            cout<<"Error in operator - : invalid Measures"<<endl;
            exit(-1);
        }
        if(_dati[0]!= Meas_2._dati[0]){
            cout<<"WARNING in operator - : subtracting 2 Measures with different lambda"<<endl;
        }
        Measure diff;
        diff._dati.push_back(_dati[0]);
        diff._dati.push_back(_dati[1] - Meas_2._dati[1]);

        if(debug){cout<<"Operator - Ends"<<endl;}
        return diff;
    };
};

vector<Measure> Difference(const vector<Measure>& M_1,const vector<Measure>& M_2){
    if(debug){cout<<"Difference Begin"<<endl;}
    if(M_1.size() != M_2.size()){
        cout<<"Error in Difference: M_1 and M_2 have different sizes"<<endl;
        exit(-1);
    }

    vector<Measure> diff;

    for(int i = 0; i<M_1.size(); i++){
        diff.push_back(M_1[i] - M_2[i]);
    }

    if(debug){cout<<"Difference Ends"<<endl;}
    return diff;
}

double Bias_Interpolator(double lambda, vector<Measure> Bias){
    if(debug){cout<<"Bias Starts"<<endl;}

    for(int i = 0; i<Bias.size();i++){
        if(lambda == Bias[i]._dati[0]){
            if(debug){cout<<"Bias Ends"<<endl;}
            return Bias[i]._dati[1];
        }
        if(lambda > Bias[i]._dati[0]){
            if(i == 0){cout<<"ERROR in bias: Lambda passed is out of range: "<<lambda<<endl; exit(-1);}
            double m = (Bias[i+1]._dati[1] - Bias[i]._dati[1])/(Bias[i+1]._dati[0] - Bias[i]._dati[0]);
            if(debug){cout<<"Bias Ends"<<endl;}
            return Bias[i+1]._dati[1]       + m * (lambda - Bias[i+1]._dati[0] );
        }
    }

    cout<<"ERROR in bias: Lambda passed is out of range "<<lambda<<endl; exit(-1);
    exit(-1);
}

void Correct(vector<Measure> input, vector<Measure> *corretti, const vector<Measure> bias){
    if(debug){cout<<"Correct Begins"<<endl;}

    for(int i=0; i<input.size(); i++){
        Measure correction;
        correction._dati.push_back(input[i]._dati[0]);
        correction._dati.push_back(input[i]._dati[1] / Bias_Interpolator(input[i]._dati[0], bias));

        corretti->push_back(correction);

    }

    if(debug){cout<<"Correct Ends"<<endl;}
}



#endif