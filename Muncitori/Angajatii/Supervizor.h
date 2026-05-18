#pragma once
#include "../Angajati.h"
#include <string>

using namespace std;

class Supervizor : public Angajat {
public:
    Supervizor(string n, string p, string cnp, string data, string oras);
    string getTip() const override;
    void Date_Angajat() const override;
};