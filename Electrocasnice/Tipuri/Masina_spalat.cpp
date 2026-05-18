#include "Masina_spalat.h"

MasinaSpalat::MasinaSpalat(string marca, string model, int an, double pret, double cap)
    : Electrocasnic("MasinaSpalat", marca, model, an, pret), capacitateKg(cap) {}

string MasinaSpalat::descriere() const {
    string info = Electrocasnic::descriere();
    // Use substr to avoid displaying too many decimals for the double
    info += " | Capacitate: " + to_string(capacitateKg).substr(0, 3) + " kg";
    return info;
}