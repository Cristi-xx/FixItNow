#pragma once
#include <iostream>
#include <map>
#include <vector>
#include <string>
#include <algorithm>

using namespace std;

class CatalogElectrocasnice {
private:
    // Structure: Type -> Brand -> [Models]
    map<string, map<string, vector<string>>> date;

public:
    // Add a new model to the service database
    void adaugaModel(string tip, string marca, string model);

    // Delete a model or an entire brand
    void stergeModel(string tip, string marca, string model);
    // Check if the appliance brought by the client can be repaired
    bool esteReparabil(string tip, string marca, string model) const;

    void stergeElement(int mod, string tip, string marca, string model);

};