#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include "Salarii/ICalculSalariu.h"
#include <ctime>

using namespace std;

class Angajat
{
protected:
    static int Contor_Id;
    int ID; // unique ID - non-const because of validations
    string nume;
    string prenume;
    string CNP;
    string data_angajarii;
    string oras_domiciliu;
    shared_ptr<ICalculSalariu> calcul_salariu;

public:
    Angajat() = default;
    Angajat(string num, string pre, string cnp, string data, string oras);
    Angajat(Angajat &) = default;
    virtual ~Angajat() = default;

    int get_Id() const;
    bool validare_CNP() const;
    bool validareNume() const;
    bool validareData() const;
    bool esteValid() const;
    int Vechime() const;
    string get_Cnp() const;
    string get_Nume() const;
    void setSistemSalarizare(shared_ptr<ICalculSalariu>);
    double Calculeaza_Salariu() const;
    string get_Domiciliu() const;
    virtual void Date_Angajat() const;
    virtual string getTip() const = 0;

    void set_Nume(string n) { nume = n; }
    void set_Prenume(string p) { prenume = p; }
};