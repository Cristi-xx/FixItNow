#include "Angajati.h"

int Angajat::Contor_Id = 0;

Angajat::Angajat(string num, string pre, string cnp, string data, string oras)
    : nume(num), prenume(pre), CNP(cnp), data_angajarii(data), oras_domiciliu(oras)
{
    if (!this->validare_CNP())
        throw std::invalid_argument("CNP INVALID");

    if (!this->validareNume())
        throw std::invalid_argument("NUME INVALID");
    if(!this->validareData())
        throw std::invalid_argument("DATA ANGAJARE INVALIDA - DEPASESTE DATA DE ASTAZI");

    ID = ++Contor_Id;
}


int Angajat::get_Id() const
{
    return ID;
}

bool Angajat::validare_CNP() const
{
    if (this->CNP.size() != 13)
        return false;
    for (int i = 0; i < 13; i++)
        if (!isdigit(CNP[i]))
            return false;

    
    int S = stoi(CNP.substr(0, 1));
    int AA = stoi(CNP.substr(1, 2));
    int LL = stoi(CNP.substr(3, 2));
    int ZZ = stoi(CNP.substr(5, 2));
    int JJ = stoi(CNP.substr(7, 2));

    // Calculate full year based on the section digit from CNP
    int an_complet = 0;
    if (S == 1 || S == 2)
        an_complet = 1900 + AA;
    else if (S == 3 || S == 4)
        an_complet = 1800 + AA;
    else if (S == 5 || S == 6)
        an_complet = 2000 + AA;

    if (S < 1 || S > 9)
        return false;
    if (LL < 1 || LL > 12)
        return false;
    if (ZZ < 1 || ZZ > 31)
        return false;
    if (!((JJ >= 1 && JJ <= 46) || JJ == 51 || JJ == 52))
        return false;

    if (LL == 4 || LL == 6 || LL == 9 || LL == 11)
    {
        if (ZZ > 30)
            return false;
    }
    else if (LL == 2)
    {
        if (ZZ > 29)
            return false;

        if (ZZ == 29)
        {
            int an_complet = 0;
            if (S == 1 || S == 2 || S >= 7)
                an_complet = 1900 + AA;
            else if (S == 3 || S == 4)
                an_complet = 1800 + AA;
            else if (S == 5 || S == 6)
                an_complet = 2000 + AA;

            bool esteBisect = (an_complet % 4 == 0 && an_complet % 100 != 0) || (an_complet % 400 == 0);
            if (!esteBisect)
                return false;
        }
    }

    int control[] = {2, 7, 9, 1, 4, 6, 3, 5, 8, 2, 7, 9};
    int suma = 0;

    for (int i = 0; i < 12; i++)
    {
        suma += (CNP[i] - '0') * control[i];
    }

    int rest = suma % 11;
    int C_calculat = (rest == 10) ? 1 : rest;
    int C_real = CNP[12] - '0';

    return (C_calculat == C_real);
}

bool Angajat::validareNume() const
{
    return (nume.length() >= 3 && nume.length() <= 30) &&
           (prenume.length() >= 3 && prenume.length() <= 30);
}

bool Angajat::esteValid() const
{
    return validareNume() && validare_CNP();
}

// Calculate salary using the decorator system
double Angajat::Calculeaza_Salariu() const
{
    if (calcul_salariu != nullptr)
        return calcul_salariu->getValoare();
    return 0.0;
}

void Angajat::setSistemSalarizare(shared_ptr<ICalculSalariu> sal)
{
    calcul_salariu = sal;
}

void Angajat::Date_Angajat() const
{
    cout << "\nDate despre angajat:\n";
    cout << "\tNUME: " << nume << "\t PRENUME:" << prenume << endl;
    cout << "\t ID:  " << ID << endl
         << "\t CNP: " << CNP << "\n\t DOMICILIU: " << oras_domiciliu << "\n \tDATA ANGAJARII:" << data_angajarii << endl;

    cout << "\tSALARIU:" << this->Calculeaza_Salariu();
}

int Angajat::Vechime() const
{
    // Get the current year
    time_t t = time(nullptr);
    tm *acum = localtime(&t);

    // Calculate the current year from the tm structure
    int an_curent = acum->tm_year + 1900;

    // Extract the hiring year from the string "YYYY-MM-DD"
    int an_angajare = stoi(this->data_angajarii.substr(6, 4));

    return an_curent - an_angajare;
}
string Angajat::get_Domiciliu() const
{
    return oras_domiciliu;
}

string Angajat::get_Cnp() const
{
    return CNP;
}
string Angajat::get_Nume() const
{
    return nume + " " + prenume;
}

bool Angajat::validareData() const {
    // 1. Get the current time (seconds since 1970)
    time_t acum = time(nullptr);

    // 2. Extract values from the string "DD-MM-YYYY" using fixed positions
    // data_angajarii format: DD (index 0), MM (index 3), YYYY (index 6)
    int zi = std::stoi(data_angajarii.substr(0, 2));
    int luna = std::stoi(data_angajarii.substr(3, 2));
    int an = std::stoi(data_angajarii.substr(6, 4));

    // 3. Populate a tm structure for the hiring date
    struct tm t_angajare = {0}; // Initialize everything to 0 (including hour/minute/second)
    
    t_angajare.tm_mday = zi;
    t_angajare.tm_mon = luna - 1;    // months in C++ are 0-11
    t_angajare.tm_year = an - 1900;  // years are counted from 1900
    t_angajare.tm_isdst = -1;        // ignore daylight savings for comparison

    // 4. Convert the above structure to time_t
    time_t timpAngajare = mktime(&t_angajare);

    // 5. Compare the two time_t values
    // If timpAngajare <= now, the date is not in the future
    return timpAngajare <= acum;

}