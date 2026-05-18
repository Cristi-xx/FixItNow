#include "Frigider.h"

Frigider::Frigider(string marca, string model, int an, double pret, bool congelator)
    : Electrocasnic("Frigider", marca, model, an, pret), areCongelator(congelator) {}

string Frigider::descriere() const {
    // Base description + fridge specific details
    string info = Electrocasnic::descriere();
    info += " | Congelator: ";
    info += (areCongelator ? "DA" : "NU");
    return info;
}