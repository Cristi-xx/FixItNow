#include "ServiceManager.h"
#include <algorithm>
#include <chrono>
#include <thread>
#include "fstream"
#include <vector>
#include "AngajatFactory.h"

time_t transformaOraManual(string textOra);

// Initialize Singleton instance
ServiceManager *ServiceManager::instanta = nullptr;

ServiceManager &ServiceManager::getInstance()
{
    if (!instanta)
        instanta = new ServiceManager();
    return *instanta;
}

bool ServiceManager::EsteServiceFunctional() const
{
    if (personal.size() < 4)
        return false;

    int nr_teh{0}, nr_sup{0}, nr_rec{0};
    for (auto &angajat : personal)
        if (angajat->getTip() == "Tehnician")
            nr_teh++;
        else if (angajat->getTip() == "Receptioner")
            nr_rec++;
        else if (angajat->getTip() == "Supervizor")
            nr_sup++;
    if (nr_teh < 3 || nr_sup < 1 || nr_rec < 1)
        return false;
    else
        return true;
}

void Creare_Salariu(shared_ptr<Angajat> angajat)
{
    // Start with base salary (4000 RON)
    shared_ptr<ICalculSalariu> lan_calcul = make_shared<SalariuDeBaza>();

    // Transport bonus if domicile is outside Bucharest
    if (angajat->get_Domiciliu() != "Bucuresti")
    {
        lan_calcul = make_shared<BonusTransport>(lan_calcul);
    }

    // Loyalty bonus: +5% of salary every 3 years
    int ani = angajat->Vechime();
    if (ani >= 3)
    {
        lan_calcul = make_shared<BonusFidelitate>(lan_calcul, ani);
    }

    // Role-specific bonuses
    if (angajat->getTip() == "Tehnician")
    {
        // Technicians receive 2% of the repair value
        shared_ptr<Tehnician> tehnician = static_pointer_cast<Tehnician>(angajat);
        lan_calcul = make_shared<BonusTehnician>(lan_calcul, tehnician);
    }
    else if (angajat->getTip() == "Supervizor")
    {
        // Supervisor: leadership bonus 20%
        lan_calcul = make_shared<SporConducere>(lan_calcul);
    }

    // Save final salary system
    angajat->setSistemSalarizare(lan_calcul);
}

// Employee management
void ServiceManager::adaugaAngajat(shared_ptr<Angajat> a)
{
    if (a)
        personal.push_back(a);
}

bool esteVarstaMinima(string cnp, string dataAngajare)
{
    // Extract birth date from CNP
    int s = cnp[0] - '0';
    int aa = stoi(cnp.substr(1, 2));
    int llNastere = stoi(cnp.substr(3, 2));
    int zzNastere = stoi(cnp.substr(5, 2));

    int anNastere = (s == 1 || s == 2) ? 1900 + aa : 2000 + aa;

    // Extract hiring date (format DD-MM-YYYY)
    int zzAngajare = stoi(dataAngajare.substr(0, 2));
    int llAngajare = stoi(dataAngajare.substr(3, 2));
    int anAngajare = stoi(dataAngajare.substr(6, 4));

    // Calculate age at hiring date
    int varsta = anAngajare - anNastere;

    // Adjust if birth day/month has not occurred yet
    if (llAngajare < llNastere || (llAngajare == llNastere && zzAngajare < zzNastere))
    {
        varsta--;
    }

    return varsta >= 16;
}

void ServiceManager::adaugaAngajatManual()
{
    DateAngajat d;
    cout << "\n--- ADAUGARE MANUALA ANGAJAT ---" << endl;

    // Read basic data
    cout << "Tip (Tehnician/Receptioner/Supervizor): ";
    cin >> d.tip;
    cout << "Nume (3-30 caractere): ";
    cin >> d.nume;
    cout << "Prenume (3-30 caractere): ";
    cin >> d.prenume;

    cout << "Data angajare (ZZ-LL-AAAA): ";
    cin >> d.dataAngajare;
    cout << "Oras domiciliu: ";
    cin >> d.oras;

    if (d.tip == "Tehnician")
    {
        int nrSpec;
        cout << "Cate specializari are tehnicianul?: ";
        cin >> nrSpec;
        for (int i = 0; i < nrSpec; i++)
        {
            string t, m;
            cout << "  Spec " << i + 1 << " - Tip: ";
            cin >> t;
            cout << "  Spec " << i + 1 << " - Marca: ";
            cin >> m;
            d.specializari.push_back({t, m});
        }
    }

    // CNP validation loop
    shared_ptr<Angajat> nouAngajat = nullptr;
    bool succes = false;

    while (!succes)
    {
        cout << "\nIntroduceti CNP-ul (13 cifre): ";
        cin >> d.cnp;
        if (existaCNP(d.cnp))
        {
            cout << "!! EROARE: Exista deja un angajat cu acest CNP in sistem!\n";
            cout << "Doriti sa incercati alt CNP? (da/nu): ";
            string rasp;
            cin >> rasp;
            if (rasp != "da")
                return;
            continue;
        }
        // Check minimum hiring age
        if (!esteVarstaMinima(d.cnp, d.dataAngajare))
        {
            cout << "!! EROARE CRITICA: Persoana are sub 16 ani la data angajarii (" << d.dataAngajare << ")!\n";
            cout << "Doriti sa incercati cu alt CNP sau alta data? (da/nu): ";
            string rasp;
            cin >> rasp;
            if (rasp != "da")
                return;
            continue; // Restart CNP loop
        }

        try
        {
            // Try to create the employee
            nouAngajat = AngajatFactory::creeazaAngajat(d);
            succes = true;
        }
        catch (const exception &e)
        {
            // Convert e.what() to string for easy comparison
            string mesaj = e.what();

            if (mesaj == "CNP INVALID")
            {
                cout << "!! EROARE: CNP-ul introdus nu este valid conform algoritmului. Doriti sa reincercati? (da/nu)\n";
                string doresc;
                cin >> doresc;
                if (doresc == "nu" || doresc == "Nu")
                    succes = true;
            }
            else if (mesaj == "NUME INVALID")
            {
                cout << "!! EROARE: Numele sau prenumele nu respecta lungimea (3-30 caractere).\n";
                cout << "Doriti sa reintroduceti numele? (da/nu): ";
                string optiune;
                cin >> optiune;
                if (optiune == "da")
                {
                    cout << "Nume nou: ";
                    cin >> d.nume;
                    cout << "Prenume nou: ";
                    cin >> d.prenume;
                }
                else
                {
                    cout << "Operatiune abandonata.\n";
                    return;
                }
            }
            else if (mesaj == "DATA ANGAJARE INVALIDA - DEPASESTE DATA DE ASTAZI")
            {
                cout <<"!!EROARE : Data de angajare depaseste data de astazi!\n";
                cout << "Doriti sa reintroduceti data de angajare? (da/nu): ";
                string optiune;
                cin >> optiune;
                if (optiune == "da")
                {
                    cout << "Data angajare (ZZ-LL-AAAA): ";
                    cin >> d.dataAngajare;
                }
                else
                {
                    cout << "Operatiune abandonata.\n";
                    return;
                }
            }
            else
            {
                // Catch any other unexpected error (e.g., memory errors)
                cout << "Eroare neasteptata: " << mesaj << ". Incercati din nou.\n";
            }
        }
    }

    // Finalize: salary and add to list
    if (nouAngajat != nullptr)
    {
        Creare_Salariu(nouAngajat); // Apply bonuses
        personal.push_back(nouAngajat);
        cout << ">>> Succes! Angajatul cu ID-ul " << nouAngajat->get_Id() << " a fost salvat.\n";
    }
}
void ServiceManager::stergeAngajat()
{
    cout << "Introdu CNP al angajatului pe care doresti sa il stergi: ";
    string cnpCautat;
    cin >> cnpCautat;
    // Iterate employee list
    for (auto it = personal.begin(); it != personal.end(); ++it)
    {

        // Check CNP
        if ((*it)->get_Cnp() == cnpCautat)
        {

            cout << "Angajatul " << (*it)->get_Nume() << " cu CNP: "
                 << cnpCautat << " a fost gasit si eliminat." << endl;

            // Remove employee from vector
            personal.erase(it);

            // Since CNP is unique, stop searching the list
            return;
        }
    }

    // If we traversed the whole list and found nothing
    cout << "Atentie: Angajatul cu CNP-ul " << cnpCautat << " NU a fost gasit in baza de date." << endl;
}
void ServiceManager::adaugaCerere(CereriReparatie c)
{
    cereriAsteptare.push_back(move(c));
}

// Request management
void ServiceManager::adaugaCerereDinFisier(string numefisier)
{
    ifstream fisier(numefisier);
    if (!fisier.is_open())
    {
        cout << "Eroare: Nu s-a putut deschide fisierul de cereri!\n";
        return;
    }

    string linie;
    getline(fisier, linie); // Ignore header line

    while (getline(fisier, linie))
    {
        if (linie.empty())
            continue;

        try
        {
            DateElectrocasnic de;
            int complexitate;
            string oraText;
            int start = 0, stop;

            // Parse fields from the line
            stop = (int)linie.find(',', start);
            de.tip = linie.substr(start, stop - start);
            start = stop + 1;
            stop = (int)linie.find(',', start);
            de.marca = linie.substr(start, stop - start);
            start = stop + 1;
            stop = (int)linie.find(',', start);
            de.model = linie.substr(start, stop - start);
            start = stop + 1;
            stop = (int)linie.find(',', start);
            de.anFabricatie = stoi(linie.substr(start, stop - start));
            start = stop + 1;
            stop = (int)linie.find(',', start);
            de.pretCatalog = stod(linie.substr(start, stop - start));
            start = stop + 1;
            stop = (int)linie.find(',', start);
            de.valNumeric = stod(linie.substr(start, stop - start));
            start = stop + 1;
            stop = (int)linie.find(',', start);
            de.atributText = linie.substr(start, stop - start);
            start = stop + 1;
            stop = (int)linie.find(',', start);
            de.optiuneBool = (linie.substr(start, stop - start) == "true");
            start = stop + 1;
            stop = (int)linie.find(',', start);
            complexitate = stoi(linie.substr(start, stop - start));
            start = stop + 1;
            oraText = linie.substr(start);

            // Validate and create appliance via factory
            unique_ptr<Electrocasnic> aparat = ElectrocasnicFactory::creeazaAparat(de, catalog);

            // Convert time format to time_t
            time_t timestamp = transformaOraManual(oraText);

            // Create request (auto-increment ID)
            CereriReparatie nouaCerere(move(aparat), complexitate, timestamp);

            // Add to waiting queue
            cereriAsteptare.push_back(move(nouaCerere));
        }
        catch (const exception &e)
        {
            cout << "Eroare la procesarea unei cereri: " << e.what() << endl;
        }
    }
    fisier.close();
    cout << "Cereri incarcate: " << cereriAsteptare.size() << endl;
}
void ServiceManager::adaugaCerereManual()
{
    DateElectrocasnic de;
    int complexitate;
    string oraText;

    cout << "\n--- ADAUGARE MANUALA CERERE REPARATIE ---\n";
    cout << "Tip aparat (Televizor/Frigider/MasinaSpalat): ";
    cin >> de.tip;
    cout << "Marca: ";
    cin >> de.marca;
    cout << "Model: ";
    cin >> de.model;
    cout << "An fabricatie: ";
    cin >> de.anFabricatie;
    cout << "Pret catalog: ";
    cin >> de.pretCatalog;

    if (de.tip == "Televizor")
    {
        cout << "Diagonala: ";
        cin >> de.valNumeric;
        cout << "Unitate (cm/inch): ";
        cin >> de.atributText;
    }
    else if (de.tip == "Frigider")
    {
        string raspuns;
        cout << "Are congelator? (da/nu): ";
        cin >> raspuns;
        de.optiuneBool = (raspuns == "da");
    }
    else if (de.tip == "MasinaSpalat")
    {
        cout << "Capacitate (kg): ";
        cin >> de.valNumeric;
    }

    cout << "Nivel complexitate (1-5): ";
    cin >> complexitate;
    cout << "Ora depunere (HH:MM): ";
    cin >> oraText;

    try
    {
        // Validate and create appliance via factory
        unique_ptr<Electrocasnic> aparat = ElectrocasnicFactory::creeazaAparat(de, catalog);

        // Convert time to time_t
        time_t timestamp = transformaOraManual(oraText);

        // Create request (duration and price are calculated automatically)
        CereriReparatie nouaCerere(move(aparat), complexitate, timestamp);

        // Add to waiting list
        cereriAsteptare.push_back(move(nouaCerere));

        cout << ">>> Cererea a fost inregistrata cu succes!\n";
    }
    catch (const invalid_argument &e)
    {
        cout << "EROARE: Modelul nu este reparabil sau datele sunt incorecte: " << e.what() << endl;
    }
    catch (const exception &e)
    {
        cout << "EROARE SISTEM: " << e.what() << endl;
    }
    catch (const char *msg)
    {
        cout << "EROARE: " << msg << endl;
    }
}

void ServiceManager::alocareAutomata()
{
    // Sort requests by submission time
    sort(cereriAsteptare.begin(), cereriAsteptare.end(), [](const CereriReparatie &a, const CereriReparatie &b)
         { return a.getTimestamp() < b.getTimestamp(); });

    auto it = cereriAsteptare.begin();
    while (it != cereriAsteptare.end())
    {
        shared_ptr<Tehnician> celMaiBunTehnician = nullptr;
        bool existaCinevaCuAceastaCompetenta = false;
        int minOreMunca = -1;

        // Find suitable technician
        for (auto &angajat : personal)
        {
            if (angajat == nullptr)
                continue;
            if (angajat->getTip() == "Tehnician")
            {
                auto teh = static_pointer_cast<Tehnician>(angajat);

                // Check competence
                if (teh->poateRepara(it->getTipAparat(), it->getMarcaAparat()))
                {
                    existaCinevaCuAceastaCompetenta = true;

                    // If they also have a free slot (max 3), consider them for assignment
                    if (teh->getNumarCereriActive() < 3)
                    {
                        if (celMaiBunTehnician == nullptr || teh->getDurataMuncaDepusa() < minOreMunca)
                        {
                            celMaiBunTehnician = teh;
                            minOreMunca = teh->getDurataMuncaDepusa();
                        }
                    }
                }
            }
        }
        // Assign receptionist if we have an available technician
        if (celMaiBunTehnician != nullptr)
        {
            shared_ptr<Receptioner> rec = getUrmatorulReceptioner();

            if (rec != nullptr)
            {
            // Receptionist registers the request
                rec->Adauga_Cerere(it->getId());

            // Technician takes the request
                it->setTehnicianAsignat(celMaiBunTehnician);
                celMaiBunTehnician->adaugareCerereActiva();
                celMaiBunTehnician->Modifica_durata_totala_munca_depusa(it->getDurata());

                cout << "[ASIGNARE] Cererea #" << it->getId() << " -> Rec: " << rec->get_Nume()
                     << " | Teh: " << celMaiBunTehnician->get_Nume() << endl;

                // Move the request to the active list and remove it from waiting
                cereriActive.push_back(move(*it));
                it = cereriAsteptare.erase(it);
                continue; // Move to next request
            }
        }
        else if (!existaCinevaCuAceastaCompetenta)
        {
            // Exists in catalog, but we have no person with the required skill
            cout << "[REJECT] Cererea #" << it->getId() << " a fost respinsa: NU AVEM TEHNICIAN SPECIALIZAT pe "
                 << it->getTipAparat() << ":" << it->getMarcaAparat() << ".\n";

            // Move it to a vector of rejected requests or remove it so the simulation is not blocked
            cereriRespinse.push_back(move(*it));
            it = cereriAsteptare.erase(it);
        }
        else
        {
            // Someone is competent, but all are busy (3/3)
            // The request remains waiting for the next tick
            ++it; // If it couldn't be assigned, go to the next one
        }
    }
}

// Simulate time step
void ServiceManager::simulareTic()
{
    cout << "\n--- Simulare Pas Timp ---" << endl;

    // Allocate new requests before processing
    alocareAutomata();

    for (auto it = cereriActive.begin(); it != cereriActive.end();)
    {
        it->proceseazaTic(); // Decrease remaining duration

        if (it->esteFinalizata())
        {
            cout << "Reparatie terminata: Cererea #" << it->getId() << endl;
            it = cereriActive.erase(it);
        }
        else
        {
            ++it;
        }
    }
}

// Status and reporting
void ServiceManager::afiseazaStatus() const
{
    cout << "\n[ STATUS SERVICE ]" << endl;
    cout << "Angajati: " << personal.size() << " | In asteptare: " << cereriAsteptare.size()
         << " | In lucru: " << cereriActive.size() << endl;

    cout << "Cereri active (status):\n";
    for (const auto &c : cereriActive)
    {
        c.afiseazaStatus();
    }
}

void ServiceManager::raportareCSV(string numeFisier) const
{
    ofstream f(numeFisier);
    if (!f.is_open())
        return;

    f << "ID,Data,Pret,Durata_Ramasa\n";
    for (const auto &c : cereriActive)
    {
        f << c.getId() << ","
          << c.getTimestamp() << ","
          << c.getPret() << ","
          << c.getDurata() << "\n";
    }
    f.close();
    cout << "Raport generat: " << numeFisier << endl;
}

// Convert time from text format to time_t
time_t transformaOraManual(string textOra)
{
    // Get current date/time
    time_t acum = time(nullptr);
    struct tm *t = localtime(&acum);

    // 2. Find where the separator is (":" or ".")
    // find_first_of searches for any of the two characters
    size_t pozitieSeparator = textOra.find_first_of(":.");

    if (pozitieSeparator != string::npos)
    {
        // Extract hours and minutes from the string
        string oreText = textOra.substr(0, pozitieSeparator);
        string minuteText = textOra.substr(pozitieSeparator + 1);

        // Convert the text to integer numbers
        int oreInt = stoi(oreText);
        int minuteInt = stoi(minuteText);

        // Set the hours and minutes
        t->tm_hour = oreInt;
        t->tm_min = minuteInt;
        t->tm_sec = 0;
    }

    // Convert to time_t
    return mktime(t);
}

void ServiceManager::Afisare_lista_angajati() const
{
    cout << "LISTA ANJAGATI: \n";
    for (auto &persoana : personal)
    {
        cout << "\t" << persoana->get_Id() << " :";
        cout << persoana->get_Nume();
        cout << endl;
    }
}

void ServiceManager::adaugaAngajatiDinFisier(string numefisier)
{
    ifstream fisier(numefisier);
    if (!fisier.is_open())
    {
        cout << "Eroare: Nu s-a putut deschide fisierul " << numefisier << endl;
        return;
    }

    string linie;
    int numarLinie = 0;

    // Ignore the first line (header)
    if (getline(fisier, linie))
    {
        numarLinie++;
    }

    while (getline(fisier, linie))
    {
        numarLinie++;
        if (linie.empty())
            continue;

        try
        {
            DateAngajat dateA;
            size_t start = 0;
            size_t stop;

            // Parse employee type
            stop = linie.find(',', start);
            dateA.tip = linie.substr(start, stop - start);
            start = stop + 1;

            // Parse last name
            stop = linie.find(',', start);
            dateA.nume = linie.substr(start, stop - start);
            start = stop + 1;

            // Parse first name
            stop = linie.find(',', start);
            dateA.prenume = linie.substr(start, stop - start);
            start = stop + 1;

            // 4. Extract CNP
            stop = linie.find(',', start);
            dateA.cnp = linie.substr(start, stop - start);
            start = stop + 1;

            if (existaCNP(dateA.cnp))
            {
                cout << "!! EROARE: Exista deja un angajat cu acest CNP in sistem!(" << dateA.cnp << ")\n";
                continue;
            }

            // extract hiring date
            stop = linie.find(',', start);
            dateA.dataAngajare = linie.substr(start, stop - start);
            start = stop + 1;

            // extract city
            stop = linie.find(',', start);
            dateA.oras = linie.substr(start, stop - start);
            start = stop + 1;

            // 7. Extract SPECIALIZATIONS (rest of the line)
            // (ex: "TV:Samsung;Frigider:Arctic")
            string rest = linie.substr(start);

            while (rest.find(':') != string::npos)
            {
                size_t pozDoua_puncte = rest.find(':');
                size_t pozPunct_Virgula = rest.find(';');

                // 1. Extract Type (everything up to ':')
                string tip = rest.substr(0, pozDoua_puncte);
                string marca;

                // 2. Extract Brand (between ':' and ';')
                if (pozPunct_Virgula != string::npos)
                {
                    // If we found ';', the brand is between the two separators
                    marca = rest.substr(pozDoua_puncte + 1, pozPunct_Virgula - pozDoua_puncte - 1);
                    // Trim the string of everything we've processed so far, including ';'
                    rest = rest.substr(pozPunct_Virgula + 1);
                }
                else
                {
                    // If we no longer have ';', we're at the last pair
                    marca = rest.substr(pozDoua_puncte + 1);
                    rest = ""; // Clear the string to stop the loop
                }

                // Add specialization
                dateA.specializari.push_back({tip, marca});
            }

            // Creation and salary
            shared_ptr<Angajat> nouAngajat = AngajatFactory::creeazaAngajat(dateA);
            Creare_Salariu(nouAngajat);
            adaugaAngajat(nouAngajat);
        }
        catch (const char *msg)
        {
            cout << "Eroare la citire: Angajat invalid pe linia " << numarLinie << ", cauza: " << msg << endl;
        }
        catch (const exception &e)
        {
            cout << "Eroare sistem pe linia " << numarLinie << ": " << e.what() << endl;
        }
    }
    fisier.close();
}

void ServiceManager::incarcaCatalogDinFisier(string numefisier)
{
    ifstream fisier(numefisier);
    if (!fisier.is_open())
    {
        cout << "Eroare: Nu s-a putut deschide fisierul de catalog " << numefisier << endl;
        return;
    }

    string linie;
    getline(fisier, linie); // Ignore header line (Type,Brand,Model)

    int count = 0;
    while (getline(fisier, linie))
    {
        if (linie.empty())
            continue;

        int start = 0;
        int stop;

        // Extract type
        stop = (int)linie.find(',', start);
        string tip = linie.substr(start, stop - start);
        start = stop + 1;

        // Extract brand
        stop = (int)linie.find(',', start);
        string marca = linie.substr(start, stop - start);
        start = stop + 1;

        // Extract model (rest of the line)
        string model = linie.substr(start);

        // Add to internal catalog
        catalog.adaugaModel(tip, marca, model);
        count++;
    }

    fisier.close();
    cout << ">>> Catalog incarcat cu succes! Modele adaugate: " << count << endl;
}

void ServiceManager::adaugaModelManual()
{
    string tip, marca, model;

    cout << "\n--- ADAUGARE MODEL NOU IN CATALOG ---\n";
    cout << "Tip aparat (ex: Televizor, Frigider, MasinaSpalat): ";
    cin >> tip;
    cout << "Marca (ex: Samsung, LG, Arctic): ";
    cin >> marca;
    cout << "Model (ex: OLED-50, Frost-X): ";
    cin >> model;

    // Add to catalog
    catalog.adaugaModel(tip, marca, model);

    cout << ">>> Modelul " << marca << " " << model << " a fost adaugat in lista de reparabile.\n";
}

shared_ptr<Receptioner> ServiceManager::getUrmatorulReceptioner()
{
    // 1. Cream o lista temporara doar cu receptionerii existenti in personal
    vector<shared_ptr<Receptioner>> listaRec;
    for (auto &angajat : personal)
    {
        if (angajat != nullptr && angajat->getTip() == "Receptioner")
        {
            listaRec.push_back(static_pointer_cast<Receptioner>(angajat));
        }
    }

    // Verific disponibilitate receptioneri
    if (listaRec.empty())
    {
        cout << "[CRITIC] Nu exista receptioneri in sistem!\n";
        return nullptr;
    }

    // 3.
    // restul impartirii la 3 va fi mereu 0, 1 sau 2.
    int pozitie = indexUltimulReceptioner % (int)listaRec.size();

    shared_ptr<Receptioner> ales = listaRec[pozitie];

    // Incrementeaza pentru urmatoarea cerere
    indexUltimulReceptioner++;

    return ales;
}

void ServiceManager::executaSimulareCompleta()
{
    int numarTic = 0;

    while (!cereriAsteptare.empty() || !cereriActive.empty())
    {
        numarTic++;
        cout << "\n========= TICUL NR. " << numarTic << " =========" << endl;

        alocareAutomata();

        for (auto itAct = cereriActive.begin(); itAct != cereriActive.end();)
        {
            itAct->proceseazaTic();

            if (itAct->esteFinalizata())
            {
                cout << "[OK] Cererea #" << itAct->getId() << " a fost finalizata!" << endl;

                // Elibereaza tehnicianul
                auto teh = itAct->getTehnicianAsignat();
                if (teh != nullptr)
                {
                    teh->retragereCerereActiva(); // Scade de la 3 la 2, etc.
                    teh->Adauga_Valoare_Reparatie(itAct->getPret());
                    cout << "   -> Tehnicianul " << teh->get_Nume() << " a eliberat un post.\n";
                }

                // Add to history
                cereriFinalizate.push_back(std::move(*itAct));

                // Remove from active
                itAct = cereriActive.erase(itAct);
            }
            else
            {
                ++itAct;
            }
        }
        this->afiseazaStatus();
    }
    cout << "\n>>> SIMULARE TERMINATA. Avem " << cereriFinalizate.size() << " reparatii in istoric.\n";
}

shared_ptr<Angajat> ServiceManager::gasesteAngajat(string identificator)
{
    for (auto &a : personal)
    {
        if (a != nullptr)
        {
            // Cautare dupa ID sau CNP
            if (to_string(a->get_Id()) == identificator || a->get_Cnp() == identificator)
            {
                return a;
            }
        }
    }
    return nullptr;
}

void ServiceManager::stergeAngajat(string identificator)
{
    auto it = remove_if(personal.begin(), personal.end(), [&](shared_ptr<Angajat> a)
                        { return a != nullptr && (to_string(a->get_Id()) == identificator || a->get_Cnp() == identificator); });

    if (it != personal.end())
    {
        personal.erase(it, personal.end());
        cout << ">>> Angajatul a fost sters cu succes.\n";
    }
    else
    {
        cout << "!! Eroare: Angajatul nu a fost gasit.\n";
    }
}

void ServiceManager::raportTop3Salarii()
{

    // Copia lista personal
    vector<shared_ptr<Angajat>> copie = personal;

    // Sortare: salariu descrescatoR
    sort(copie.begin(), copie.end(), [](const shared_ptr<Angajat> &a, const shared_ptr<Angajat> &b)
         {
             if (a->Calculeaza_Salariu() != b->Calculeaza_Salariu())
             {
                 return a->Calculeaza_Salariu() > b->Calculeaza_Salariu();
             }
             return a->get_Nume() < b->get_Nume(); });

    // Scriere in CSV
    ofstream f("raport_top3_salarii.csv");
    f << "Nume si Prenume,Salariu\n";

    int limita = (copie.size() < 3) ? (int)copie.size() : 3;
    for (int i = 0; i < limita; i++)
    {
        f << copie[i]->get_Nume() << "," << copie[i]->Calculeaza_Salariu() << "\n";
    }
    f.close();
    cout << ">>> Raport generat: raport_top3_salarii.csv\n";
}

void ServiceManager::raportTehnicianRecord()
{
    if (cereriFinalizate.empty())
    {
        cout << "Nu exista reparații finalizate pentru raport.\n";
        return;
    }

    // Find the request with the longest duration
    auto it = max_element(cereriFinalizate.begin(), cereriFinalizate.end(), [](CereriReparatie &a, CereriReparatie &b)
                          { return a.getDurataInitiala() < b.getDurataInitiala(); });

    // Extract the technician who worked on it
    auto teh = it->getTehnicianAsignat();

    if (teh != nullptr)
    {
        ofstream f("raport_tehnician_record.csv");
        f << "Nume Tehnician,ID,CNP,Durata Reparatie (ore),Aparat\n";
        f << teh->get_Nume() << ","
          << teh->get_Id() << ","
          << teh->get_Cnp() << ","
          << it->getDurataInitiala() << ","
          << it->getMarcaAparat() << " " << it->getModelAparat() << "\n";
        f.close();
        cout << ">>> Raport generat: raport_tehnician_record.csv\n";
    }
}
void ServiceManager::raportCereriAsteptareGrupate()
{
    // Creeaza vector pointeri catre cereri
    vector<const CereriReparatie *> pointeriCereri;
    for (const auto &c : cereriAsteptare)
    {
        pointeriCereri.push_back(&c);
    }

    // Sorteaza pointerii (nu obiectele)
    sort(pointeriCereri.begin(), pointeriCereri.end(), [](const CereriReparatie *a, const CereriReparatie *b)
         {
        if (a->getTipAparat() != b->getTipAparat())
            return a->getTipAparat() < b->getTipAparat();
        if (a->getMarcaAparat() != b->getMarcaAparat())
            return a->getMarcaAparat() < b->getMarcaAparat();
        return a->getModelAparat() < b->getModelAparat(); });

    // Genereaza CSV
    ofstream f("raport_cereri_asteptare.csv");
    f << "Tip,Marca,Model,ID Cerere,Timp Estimat\n";
    for (const auto *c : pointeriCereri)
    {
        f << c->getTipAparat() << ","
          << c->getMarcaAparat() << ","
          << c->getModelAparat() << ","
          << c->getId() << ","
          << c->getDurata() << "\n";
    }
    f.close();
    cout << ">>> Raport generat: raport_cereri_asteptare.csv\n";
}

bool ServiceManager::existaCNP(const string &cnpCautat)
{
    for (const auto &angajat : personal)
    {
        if (angajat != nullptr && angajat->get_Cnp() == cnpCautat)
        {
            return true;
        }
    }
    return false;
}

void ServiceManager::proceseazaUnTic(int &numarTicCurent)
{
    numarTicCurent++;
    cout << "\n>>>>>> EXECUTARE TIC NR. " << numarTicCurent << " <<<<<<" << endl;

    // Alocare automata
    alocareAutomata();

    // Procesare munca
    if (cereriActive.empty())
    {
        cout << "Nicio cerere in lucru in acest moment.\n";
    }

    for (auto itAct = cereriActive.begin(); itAct != cereriActive.end();)
    {
        itAct->proceseazaTic(); // durataEstimata--

        if (itAct->esteFinalizata())
        {
            cout << "[FINALIZAT] Cererea #" << itAct->getId() << " a fost reparata!";

            auto teh = itAct->getTehnicianAsignat();
            if (teh != nullptr)
            {
                teh->retragereCerereActiva();
                // Actualizeaza salariul
                teh->Adauga_Valoare_Reparatie(itAct->getPret());
                cout << " -> Tehnician: " << teh->get_Nume() << " (Suma adaugata la palmares)";
            }
            cout << endl;

            cereriFinalizate.push_back(std::move(*itAct));
            itAct = cereriActive.erase(itAct);
        }
        else
        {
            // Afiseaza durata ramasa
            cout << " - Cererea #" << itAct->getId() << " (" << itAct->getMarcaAparat()
                 << "): " << itAct->getDurata() << " ore ramase.\n";
            ++itAct;
        }
    }

    // Afiseaza status final
    cout << "Status: " << cereriAsteptare.size() << " in asteptare | "
         << cereriActive.size() << " active.\n";
}

void ServiceManager::stergeDinCatalog(int mod, string t, string m, string mo)
{
    catalog.stergeElement(mod, t, m, mo);
}

void ServiceManager::afisare_completa_angajati() const
{
    cout << endl;
    for (auto persoana : personal)
        persoana->Date_Angajat();
}
