#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <ctime>

using namespace std;

class Electrocasnic {
    string tip;
    string marca;
    string model;
    int an_fabricatie;
    double pret_catalog;
public:
    virtual ~Electrocasnic()=default;
    Electrocasnic(string t, string m, string mod, int an, double p);
    int get_Vechime() const;
    string getTip() const;
    string getMarca() const;
    string getModel() const;
    double getPret() const;
    virtual string descriere() const;
};

