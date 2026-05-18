#pragma once

#include "../Electrocasnic.h"

class Televizor : public Electrocasnic {
private:
    double diagonala;
    string unitate; // "cm" or "inch"

public:
    Televizor(string marca, string model, int an, double pret,
              double diag, string u);
    string descriere() const override;
};
