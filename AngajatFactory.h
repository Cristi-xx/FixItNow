#pragma once
#include <memory>
#include <string>
#include "Muncitori/Angajati.h"
#include "Muncitori/Angajatii/Receptioner.h"
#include "Muncitori/Angajatii/Tehnician.h"
#include "Muncitori/Angajatii/Supervizor.h"
// just to make the pointer type dynamic for specific employee types
using namespace std;

struct DateAngajat {
    // Personal data
    string tip;
    string nume;
    string prenume;
    string cnp;
    string dataAngajare;
    string oras;

    // The only static "skill" required at hire time
    vector<pair<string, string>> specializari; // For technician
};

class AngajatFactory {
public:
    static shared_ptr<Angajat> creeazaAngajat(const DateAngajat& d);
    // Metoda statica a factory-ului
};