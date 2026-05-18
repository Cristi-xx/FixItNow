#include "Supervizor.h"

Supervizor::Supervizor(string n, string p, string cnp, string data, string oras)
    : Angajat(n, p, cnp, data, oras) {}

string Supervizor::getTip() const {
    return "Supervizor";
}
void Supervizor::Date_Angajat() const
{
    Angajat::Date_Angajat();
    cout<<"\n\tOCUPATIE: SUPERVIZOR\n";
}