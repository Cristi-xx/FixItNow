#pragma once
#include "../Angajati.h"
#include <vector>
#include <string>

using namespace std;

class Receptioner : public Angajat {
private:
    vector<int> ID_cereri_inregistrate;

public:
    Receptioner(string n, string p, string cnp, string data, string oras);
    void Adauga_Cerere(int ID);
    void Date_Angajat() const override;
    string getTip() const override;
};