
#ifndef Measure_h
#define Measure_h

#include "General.h"

class Measure{
    public:
    Measure(){;}
    Measure(const double &lambda, const double &value){_lambda = lambda; _value = value;}

    void SetLambda(const double& lambda){_lambda = lambda;};
    void SetValue(const  double& value){_value = value;};

    double GetLambda() const {return _lambda;};
    double GetValue() const {return _value;};

    void Print() const {
        cout<<_lambda<<"\t\t"<<_value<<endl;
    };
    void Print(ofstream *out, bool calib = 0) const {
        if(calib){
            *out<<"lambda:\t"<<_lambda<<endl;
            *out<<"Abs:\t"<<_value<<endl;
        }
        else{*out<<_lambda<<"\t\t"<<_value<<endl;}
    };

    Measure& operator=(const Measure& other) {
        if (this != &other) { // Check for self-assignment
            _lambda = other._lambda;
            _value = other._value;
        }
        return *this;
    }

    Measure operator-(const Measure& other) const {
        // Subtract the values of the two Measure objects
        double subtracted_value = _value - other._value;

        // Create and return a new Measure object with the result
        return Measure(_lambda, subtracted_value);
    }

    private:
    double _lambda;
    double _value;
};



#endif