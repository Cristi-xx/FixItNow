#include "ElectrocasnicFactory.h"
#include "Electrocasnice/Tipuri/Frigider.h"
#include "Electrocasnice/Tipuri/TV.h"
#include "Electrocasnice/Tipuri/Masina_spalat.h"
#include <stdexcept>

unique_ptr<Electrocasnic> ElectrocasnicFactory::creeazaAparat(const DateElectrocasnic& d, const CatalogElectrocasnice& catalog) {
    // 1. Validation
    if (!catalog.esteReparabil(d.tip, d.marca, d.model)) {
        throw invalid_argument("Model nereparabil: " + d.tip + " " + d.marca);
    }

    // Create specific appliance
    if (d.tip == "Frigider") {
        return make_unique<Frigider>(d.marca, d.model, d.anFabricatie, d.pretCatalog, d.optiuneBool);
    }

    if (d.tip == "Televizor") {
        return make_unique<Televizor>(d.marca, d.model, d.anFabricatie, d.pretCatalog, d.valNumeric, d.atributText);
    }

    if (d.tip == "MasinaSpalat") {
        return make_unique<MasinaSpalat>(d.marca, d.model, d.anFabricatie, d.pretCatalog, d.valNumeric);
    }

    throw invalid_argument("Tip necunoscut: " + d.tip);
}