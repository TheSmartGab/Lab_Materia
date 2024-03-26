#ifndef __Measures_h__
#define __Measures_h__


#include <iostream>
#include <vector>

using namespace std;

#ifndef __Debug__
#define __Debug__
static int debug = false;
#endif

struct Measure{
    vector<double> _dati;
};

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