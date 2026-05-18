#pragma once
#include <memory>
#include <string>
#include "Electrocasnice/Electrocasnic.h"
#include "Electrocasnice/CatalogElectrocasnice.h"

using namespace std;

// Structure for appliance data - used by the factory
struct DateElectrocasnic {
    string tip;
    string marca;
    string model;
    int anFabricatie;
    double pretCatalog;
    double valNumeric; // for kilograms or diagonal (if TV or washing machine)
    string atributText; // unit of measure for TV diagonal
    bool optiuneBool;   // whether the fridge has a freezer or not
};

class ElectrocasnicFactory {
public:
    static unique_ptr<Electrocasnic> creeazaAparat(const DateElectrocasnic& d, const CatalogElectrocasnice& catalog);
};