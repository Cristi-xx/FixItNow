#include "ICalculSalariu.h"
#include "../Angajatii/Tehnician.h"

// Base salary: 4000 RON
double SalariuDeBaza::getValoare() const {
    return 4000.0; 
}

// Abstract decorator
Decorator_Salariu::Decorator_Salariu(shared_ptr<ICalculSalariu> comp) 
    : componenta(comp) {}

double Decorator_Salariu::getValoare() const {
    return componenta->getValoare();
}

// +5% bonus every 3 years of work
BonusFidelitate::BonusFidelitate(shared_ptr<ICalculSalariu> comp, int ani)
    : Decorator_Salariu(comp), ani_vechime(ani) {}

double BonusFidelitate::getValoare() const {
    double bonus = (4000.0 * 0.05) * (ani_vechime / 3);
    return componenta->getValoare() + bonus;
}

// +400 RON transport bonus
BonusTransport::BonusTransport(shared_ptr<ICalculSalariu> comp)
    : Decorator_Salariu(comp) {}

double BonusTransport::getValoare() const {
    return componenta->getValoare() + 400.0;
}

// +2% of the repair value
BonusTehnician::BonusTehnician(shared_ptr<ICalculSalariu> comp, shared_ptr<Tehnician> teh)
    : Decorator_Salariu(comp), tehnician(teh) {}

double BonusTehnician::getValoare() const {
    return componenta->getValoare() + (tehnician->getValoareReparatii() * 0.02);
}

// +20% leadership bonus (Supervisor)
SporConducere::SporConducere(shared_ptr<ICalculSalariu> comp)
    : Decorator_Salariu(comp) {}

double SporConducere::getValoare() const {
    return componenta->getValoare() + (4000.0 * 0.20);
}