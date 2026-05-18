#pragma once
#include <vector>
#include <memory>
#include "Electrocasnice/CatalogElectrocasnice.h"
#include "Cereri/CereriReparatie.h"
#include "Muncitori/Angajati.h"
#include "AngajatFactory.h"
#include "ElectrocasnicFactory.h"

using namespace std;

class ServiceManager
{
private:
    vector<shared_ptr<Angajat>> personal;
    vector<CereriReparatie> cereriAsteptare;
    vector<CereriReparatie> cereriActive;
    vector<CereriReparatie> cereriFinalizate;
    vector<CereriReparatie> cereriRespinse; // due to lack of staff
    CatalogElectrocasnice catalog;

    // Singleton: private constructor
    ServiceManager() {}
    static ServiceManager *instanta;

    // Disable copying
    ServiceManager(const ServiceManager &) = delete;
    ServiceManager &operator=(const ServiceManager &) = delete;

    //
    int indexUltimulReceptioner = 0;

public:
    static ServiceManager &getInstance();
    bool EsteServiceFunctional() const;

    // Management methods according to the Architecture
    // About employees
    void adaugaAngajat(shared_ptr<Angajat>);
    void adaugaAngajatManual();
    void adaugaAngajatiDinFisier(string numefisier);
    void stergeAngajat(); // by CNP
    void Afisare_lista_angajati() const;
    void afisare_completa_angajati() const;
    bool existaCNP(const string& cnpCautat);


    // About requests
    void adaugaCerere(CereriReparatie);            // add to list
    void adaugaCerereDinFisier(string numefisier); // Receives an already created request
    void adaugaCerereManual();

    // About catalog
    void incarcaCatalogDinFisier(string numefisier);
    void adaugaModelManual();
    void stergeDinCatalog(int mod, string tip, string marca, string model);


    // Simulare asignare cereri
    void executaSimulareCompleta();
    void simulareTic();
    void alocareAutomata();
    void afiseazaStatus() const;
    void proceseazaUnTic(int& numarTicCurent);


    // asignare receptioner
    shared_ptr<Receptioner> getUrmatorulReceptioner();

    // Pentru meniu angajati
    shared_ptr<Angajat> gasesteAngajat(string identificator);
    void stergeAngajat(string identificator);
    void raportTop3Salarii();
    void raportTehnicianRecord();
    void raportCereriAsteptareGrupate();
    void raportareCSV(string numeFisier) const;

    //Getteri
    size_t getCereriAsteptareSize() const { return cereriAsteptare.size();};
    size_t getCereriActiveSize() const { return cereriActive.size();};



};