#include "Tehnician.h"

Tehnician::Tehnician(string n, string p, string cnp, string data, string oras,
                     const vector<pair<string,string>>& sp)
    : Angajat(n, p, cnp, data, oras), specializari(sp), valoare_totala_reparatii(0.0), numar_cereri_active(0) {}

void Tehnician::adauga_Specializare(const pair<string, string>& pereche) {
    specializari.push_back(pereche);
}

double Tehnician::getValoareReparatii() const {
    return valoare_totala_reparatii;
}

int Tehnician::getNumarCereriActive() const {
    return numar_cereri_active;
}

string Tehnician::getTip() const {
    return "Tehnician";
}

void Tehnician::adaugareCerereActiva() {
    numar_cereri_active++;
}
void Tehnician::retragereCerereActiva() {
    numar_cereri_active--;
}

void Tehnician::Date_Angajat() const{
    Angajat::Date_Angajat();
    cout<<"\n \t OCUPATIE : TEHNICIAN";
    cout<<"\n \t SPECIALIZARI: ";
    bool primul = true; 
    for(auto const& pereche : specializari) {
        if (!primul) {
            cout << "; ";
        }
        cout << pereche.first << " - " << pereche.second;
        primul = false; 
    }
    cout<<endl<<"Valoare totala reparatii: "<<valoare_totala_reparatii<<endl<<"Numar cereri active: "<<numar_cereri_active<<endl<<endl;
}

void Tehnician::Modifica_durata_totala_munca_depusa(int ore){
    durata_totala_munca_depusa=durata_totala_munca_depusa+ore;
}

bool Tehnician::poateRepara(string tip, string marca) const {
    for (const auto& spec : specializari) {
        if (spec.first == tip && spec.second == marca) {
            return true;
        }
    }
    return false;
}

int Tehnician::getDurataMuncaDepusa() const {
    return durata_totala_munca_depusa;
}

void Tehnician::Adauga_Valoare_Reparatie(double pret){
    valoare_totala_reparatii=valoare_totala_reparatii+pret;
}
