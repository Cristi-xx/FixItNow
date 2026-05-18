#include "Electrocasnic.h"

Electrocasnic::Electrocasnic(string t, string m, string mod, int an, double p) 
    : tip(t), marca(m), model(mod), an_fabricatie(an), pret_catalog(p) {}

int Electrocasnic::get_Vechime() const {
    // Get current time
    time_t t = time(nullptr);
    tm *acum = localtime(&t);

    // Calculate current year
    int an_curent = acum->tm_year + 1900;

    return an_curent - an_fabricatie;
}

string Electrocasnic::getTip() const {
    return tip;
}

string Electrocasnic::getMarca() const {
    return marca;
}

string Electrocasnic::getModel() const {
    return model;
}

string Electrocasnic::descriere() const {
    return tip + " " + marca + " " + model;
}
double Electrocasnic::getPret() const{
    return pret_catalog;
}