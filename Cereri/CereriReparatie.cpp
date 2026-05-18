
#include "CereriReparatie.h"
#include <string>

int CereriReparatie::ContorID = 1;

using namespace std;

// Constructor: implements calculation logic according to requirements
CereriReparatie::CereriReparatie(unique_ptr<Electrocasnic> apar, int complex, time_t oraDinFisier=0)
    : aparat(move(apar))
{
    this->id = CereriReparatie::ContorID;

    CereriReparatie::ContorID = CereriReparatie::ContorID + 1;

    if (complex == 0)
        throw "Aparatul nu poate fi reparat!";
    // Set current timestamp or use the time from file
    if (oraDinFisier == 0)
    {
        this->timestamp = time(nullptr);
    }
    else
    {
        this->timestamp = oraDinFisier;
    }

    // Normalize complexity to range 1-5
    if (complex < 1)
        complexitate = 1;
    else if (complex > 5)
        complexitate = 5;
    else
        complexitate = complex;

    // Duration = appliance age * complexity
    this->durataEstimata = aparat->get_Vechime() * complexitate;
    this->durataInitiala=durataEstimata;

    // Price = catalog price * duration
    this->pretTotal = aparat->getPret() * this->durataEstimata;
}

void CereriReparatie::proceseazaTic()
{
    if (durataEstimata > 0)
    {
        durataEstimata--;
    }
}

bool CereriReparatie::esteFinalizata() const
{
    return durataEstimata <= 0;
}

void CereriReparatie::afiseazaStatus() const
{
    time_t temp = timestamp;
    string dataStr = ctime(&temp);

    // Remove trailing newline
    if (!dataStr.empty() && dataStr.back() == '\n')
        dataStr.pop_back();

    cout << "ID: " << id << " | Data: " << dataStr << endl;
    cout << "Aparat: " << aparat->getMarca() << " " << aparat->getModel() << endl;
    cout << "Durata ramasa: " << durataEstimata << " | Pret: " << pretTotal << " RON" << endl;
    cout << "---------------------------------------" << endl;
}

// Getteri
int CereriReparatie::getId() const { return id; }

double CereriReparatie::getPret() const { return pretTotal; }

time_t CereriReparatie::getTimestamp() const { return timestamp; }

int CereriReparatie::getDurata() const { return durataEstimata; }

int CereriReparatie::getVechime() const
{
    return aparat->get_Vechime();
}

string CereriReparatie::getTipAparat() const {
    return aparat->getTip();
}

string CereriReparatie::getMarcaAparat() const {
    return aparat->getMarca();
}

void CereriReparatie::setTehnicianAsignat(shared_ptr<Tehnician> t) {
    tehnicianAsignat = t;
}

shared_ptr<Tehnician> CereriReparatie::getTehnicianAsignat() {
    return tehnicianAsignat;
}

string CereriReparatie::getModelAparat() const
{
    return aparat->getModel();
}
int CereriReparatie::getDurataInitiala() const
{
    return durataInitiala;
}
