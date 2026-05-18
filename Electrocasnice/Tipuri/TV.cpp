#include "TV.h"

Televizor::Televizor(string marca, string model, int an, double pret,
              double diag, string u)
    : Electrocasnic("Televizor", marca, model, an, pret),
      diagonala(diag), unitate(u) {}

string Televizor::descriere() const {
    string info = Electrocasnic::descriere();
    info += " | Diagonala: " + to_string(diagonala).substr(0, 4) + " " + unitate;
    return info;
}