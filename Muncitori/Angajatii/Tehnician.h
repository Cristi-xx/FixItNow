#pragma once
#include "../Angajati.h"
#include <iostream>
#include <utility>
#include <vector>
#include <string>

using namespace std;

class Tehnician : public Angajat {
private:
    vector<pair<string,string>> specializari;
    double valoare_totala_reparatii{0};
    int numar_cereri_active{0};
    int durata_totala_munca_depusa{0};

public:
    Tehnician()=default;
    Tehnician(string n, string p, string cnp, string data, string oras,const vector<pair<string,string>>&);
    void adauga_Specializare(const pair<string, string>&);
    double getValoareReparatii() const;
    int getNumarCereriActive() const;
    void adaugareCerereActiva();
    void retragereCerereActiva();
    void Modifica_durata_totala_munca_depusa(int ore);
    int getDurataMuncaDepusa() const;
    void Date_Angajat() const override;
    bool poateRepara(string tip, string marca) const;
    void Adauga_Valoare_Reparatie(double);

    string getTip() const override;
};