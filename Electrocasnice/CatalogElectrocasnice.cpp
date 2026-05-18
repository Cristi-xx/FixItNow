#include "CatalogElectrocasnice.h"

void CatalogElectrocasnice::adaugaModel(string tip, string marca, string model) {
    date[tip][marca].push_back(model);
}

void CatalogElectrocasnice::stergeModel(string tip, string marca, string model) {
    if (date.count(tip) && date[tip].count(marca)) {
        auto& modele = date[tip][marca];
        modele.erase(remove(modele.begin(), modele.end(), model), modele.end());
    }
}

bool CatalogElectrocasnice::esteReparabil(string tip, string marca, string model) const {
    // Check existence of the type
    auto itTip = date.find(tip);
    if (itTip == date.end()) return false;

    // Check brand
    auto itMarca = itTip->second.find(marca);
    if (itMarca == itTip->second.end()) return false;

    // Check model
    const auto& modele = itMarca->second;
    return find(modele.begin(), modele.end(), model) != modele.end();
}

void CatalogElectrocasnice::stergeElement(int mod, string tip, string marca, string model) {
    auto itTip = date.find(tip);
    if (itTip == date.end()) {
        cout << "!! Tipul [" << tip << "] nu exista in catalog.\n";
        return;
    }

    switch (mod) {
        case 1: // Delete type and everything associated
            date.erase(itTip);
            cout << ">>> Tipul [" << tip << "] si tot ce tinea de el a fost sters.\n";
            break;

        case 2: { // Delete brand and its models
            auto& marci = itTip->second;
            if (marci.erase(marca)) {
                cout << ">>> Marca [" << marca << "] de la tipul [" << tip << "] a fost stearsa.\n";
                // Cleanup: if the type remains without brands, remove it
                if (marci.empty()) date.erase(itTip);
            } else {
                cout << "!! Marca [" << marca << "] nu exista sub tipul [" << tip << "].\n";
            }
            break;
        }

        case 3: { // Delete a specific model
            auto& marci = itTip->second;
            if (marci.count(marca)) {
                auto& modele = marci[marca]; // Vector of models
                auto itMod = find(modele.begin(), modele.end(), model);
                
                if (itMod != modele.end()) {
                    modele.erase(itMod);
                    cout << ">>> Modelul [" << model << "] a fost eliminat.\n";
                    
                    // Cleanup: remove the brand if it has no models left
                    if (modele.empty()) marci.erase(marca);
                    // Remove the type if it has no brands left
                    if (marci.empty()) date.erase(itTip);
                } else {
                    cout << "!! Modelul [" << model << "] nu a fost gasit.\n";
                }
            } else {
                cout << "!! Marca [" << marca << "] nu exista.\n";
            }
            break;
        }
    }
}