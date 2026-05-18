#include "Receptioner.h"

Receptioner::Receptioner(string n, string p, string cnp, string data, string oras)
    : Angajat(n, p, cnp, data, oras) {}

void Receptioner::Adauga_Cerere(int ID) {
    ID_cereri_inregistrate.push_back(ID);
}

string Receptioner::getTip() const {
    return "Receptioner";
}

void Receptioner::Date_Angajat() const
{
    Angajat::Date_Angajat();
    cout<<"\n\tOCUPATIE :  RECEPTIONER";
    cout<<"\n\t ID cereri inregistrate:";
    for (auto& obj : ID_cereri_inregistrate)
        cout<<obj<<", ";
    cout<<endl;

}