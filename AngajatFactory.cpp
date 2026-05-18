#include "AngajatFactory.h"
#include <stdexcept>
#include <ctime>

shared_ptr<Angajat> AngajatFactory::creeazaAngajat(const DateAngajat &d)
{

    // For Technician: requires specializations
    if (d.tip == "Tehnician")
    {
        return make_shared<Tehnician>(d.nume, d.prenume, d.cnp, d.dataAngajare, d.oras, d.specializari);
    }

    // For Receptionist: basic data
    // Their ID vector will be initialized EMPTY in the Receptioner constructor.
    if (d.tip == "Receptioner")
    {
        return make_shared<Receptioner>(d.nume, d.prenume, d.cnp, d.dataAngajare, d.oras);
    }

    // For Supervisor: basic data
    if (d.tip == "Supervizor")
    {
        return make_shared<Supervizor>(d.nume, d.prenume, d.cnp, d.dataAngajare, d.oras);
    }

    throw invalid_argument("Tip de angajat necunoscut: " + d.tip);
}