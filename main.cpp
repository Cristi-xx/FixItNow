#include <iostream>
#include <vector>
#include <memory>
#include "ServiceManager.h"
#include "Muncitori/Angajatii/Tehnician.h"
#include "Muncitori/Salarii/ICalculSalariu.h"

using namespace std;

int citesteOptiune()
{
    int opt;
    while (!(cin >> opt))
    {
        cout << "Eroare: Introduceti o cifra valida: ";
        cin.clear();
        cin.ignore(1000, '\n');
    }
    return opt;
}

inline time_t transformaOraManual(string textOra)
{
    // Take current date/time as reference
    time_t acum = time(nullptr);
    struct tm *t = localtime(&acum);

    // Find the separator between hours and minutes
    size_t pozitieSeparator = textOra.find_first_of(":." );

    if (pozitieSeparator != string::npos)
    {
        // Extract hours and minutes from text
        string oreText = textOra.substr(0, pozitieSeparator);
        string minuteText = textOra.substr(pozitieSeparator + 1);

        int oreInt = stoi(oreText);
        int minuteInt = stoi(minuteText);

        // Set hours and minutes in the tm structure
        t->tm_hour = oreInt;
        t->tm_min = minuteInt;
        t->tm_sec = 0;
    }

    // Convert to time_t
    return mktime(t);
}

void subMeniuActiuniAngajat(ServiceManager &manager)
{
    string iden;
    cout << "\nIntroduceti ID-ul sau CNP-ul angajatului: ";
    cin >> iden;

    shared_ptr<Angajat> a = manager.gasesteAngajat(iden);
    if (a == nullptr)
    {
        cout << "!! Angajatul nu a fost gasit.\n";
        return;
    }

    int optiune;
    while (true)
    {
        cout << "\n--- ACTIUNI PENTRU: " << a->get_Nume() << "----\n";
        cout << "1. Vizualizare date complete\n";
        cout << "2. Modifica Nume/Prenume\n";
        cout << "3. STERGE Angajatul din sistem\n";
        cout << "0. Revenire la lista\n";
        cout << "Alegerea dvs: ";

        if (!(cin >> optiune))
        {
            cin.clear();
            cin.ignore(1000, '\n');
            continue;
        }
        if (optiune == 0)
            break;

        switch (optiune)
        {
        case 1:
            a->Date_Angajat();
            break;
        case 2:
        {
            string nNou, pNou;
            cout << "Nume nou: ";
            cin >> nNou;
            cout << "Prenume nou: ";
            cin >> pNou;
            a->set_Nume(nNou);
            a->set_Prenume(pNou);
            cout << ">>> Date actualizate!\n";
            break;
        }
        case 3:
        {
            cout << "Sigur doriti stergerea? (da/nu): ";
            string confirm;
            cin >> confirm;
            if (confirm == "da")
            {
                manager.stergeAngajat(iden);
                return; // Exit the sub-menu
            }
            break;
        }
        }
    }
}

void meniuAngajati(ServiceManager &manager)
{
    bool inapoi = false;
    while (!inapoi)
    {
        cout << "\n--- GESTIUNE ANGAJATI ---";
        cout << "\n1. Incarcare din fisier CSV";
        cout << "\n2. Adaugare manuala angajat";
        cout << "\n3. Afisare tot personalul ";
        cout << "\n4. CAUTA / MODIFICA / STERGE";
        cout << "\n0. Revenire la meniul principal";
        cout << "\nAlegerea dvs: ";

        int opt = citesteOptiune();
        switch (opt)
        {
        case 1:
        {
            string numefisier;
            cout << "Introduceti numele fisierului:";
            cin >> numefisier;
            numefisier = "tests/" + numefisier;
            manager.adaugaAngajatiDinFisier(numefisier);
            break;
        }
        case 2:
            manager.adaugaAngajatManual();
            break;
        case 3:
        {
            cout << "\n Doriti afisarea \n\t1: ID: Nume + Prenume  \n SAU \n \t2: Toate detaliile!\n Alege 1 sau 2.\n";
            int x = 0;
            cin >> x;
            if (x == 1)
                manager.Afisare_lista_angajati();
            else if (x == 2)
                manager.afisare_completa_angajati();
            else
                cout << "Ati introdus altceva, deci nu se va afisa nimic.";
            break;
        }
        case 4:
            subMeniuActiuniAngajat(manager);
            break;
        case 0:
            inapoi = true;
            break;
        default:
            cout << "\n!! Optiune invalida!";
            break;
        }
    }
}

void meniuStergereCatalog(ServiceManager &manager)
{
    bool inapoi = false;
    while (!inapoi)
    {
        cout << "\n=========== STERGE DIN CATALOG ===========";
        cout << "\n1. STERGE TIP (Inclusiv marci si modele)";
        cout << "\n2. STERGE MARCA (Inclusiv modele)";
        cout << "\n3. STERGE MODEL SPECIFIC";
        cout << "\n0. REVENIRE";
        cout << "\n==========================================";
        cout << "\nAlegerea dvs: ";

        int opt = citesteOptiune();
        string t, m, mo;

        switch (opt)
        {
        case 1:
            cout << "Tip (ex: TV): ";
            cin >> t;
            manager.stergeDinCatalog(1, t, "", "");
            break;
        case 2:
            cout << "Tip: ";
            cin >> t;
            cout << "Marca de sters (ex: Samsung): ";
            cin >> m;
            manager.stergeDinCatalog(2, t, m, "");
            break;
        case 3:
            cout << "Tip: ";
            cin >> t;
            cout << "Marca: ";
            cin >> m;
            cout << "Model de sters: ";
            cin >> mo;
            manager.stergeDinCatalog(3, t, m, mo);
            break;
        case 0:
            inapoi = true;
            break;
        default:
            cout << "\n!! Optiune invalida!";
            break;
        }
    }
}

void meniuCatalog(ServiceManager &manager)
{
    bool inapoi = false;
    while (!inapoi)
    {
        cout << "\n--- GESTIUNE CATALOG ---";
        cout << "\n1. Incarcare catalog din fisier";
        cout << "\n2. Adaugare model manual";
        cout << "\n3. Stergere model";
        cout << "\n0. Revenire la meniul principal";
        cout << "\nAlegerea dvs: ";

        int opt = citesteOptiune();
        switch (opt)
        {
        case 1:
        {
            string numefisier;
            cout << "Introduceti numele fisierului:";
            cin >> numefisier;
            numefisier = "tests/" + numefisier;
            manager.incarcaCatalogDinFisier(numefisier);
            break;
        }
        case 2:
            manager.adaugaModelManual();
            break;
        case 3:
            meniuStergereCatalog(manager);
            break;
        case 0:
            inapoi = true;
            break;
        default:
            cout << "\n!! Optiune invalida!";
            break;
        }
    }
}

void meniuCereri(ServiceManager &manager)
{
    bool inapoi = false;
    while (!inapoi)
    {
        cout << "\n--- GESTIUNE CERERI ---";
        cout << "\n1. Incarcare cereri din fisier";
        cout << "\n2. Adaugare cerere manuala";
        cout << "\n3. Status service (Coadă/Ocupare)";
        cout << "\n0. Revenire la meniul principal";
        cout << "\nAlegerea dvs: ";

        int opt = citesteOptiune();
        switch (opt)
        {
        case 1:
        {
            string numefisier;
            cout << "Introduceti numele fisierului:";
            cin >> numefisier;
            numefisier = "tests/" + numefisier;
            manager.adaugaCerereDinFisier(numefisier);
            break;
        }
        case 2:
            manager.adaugaCerereManual();
            break;
        case 3:
            manager.afiseazaStatus();
            break;
        case 0:
            inapoi = true;
            break;
        default:
            cout << "\n!! Optiune invalida!";
            break;
        }
    }
}
void meniuRaportari(ServiceManager &manager)
{
    bool inapoi = false;
    while (!inapoi)
    {
        cout << "\n--- MODUL RAPOARTE CSV ---";
        cout << "\n1. Top 3 Salarii ";
        cout << "\n2. Tehnicianul cu durata maxima";
        cout << "\n3. Cereri asteptare (Grupate)";
        cout << "\n4. Raport cereri active";
        cout << "\n0. Revenire la meniul principal";
        cout << "\nAlegerea dvs: ";

        int opt = citesteOptiune();
        switch (opt)
        {
        case 1:
            manager.raportTop3Salarii();
            break;
        case 2:
            manager.raportTehnicianRecord();
            break;
        case 3:
            manager.raportCereriAsteptareGrupate();
            break;
        case 4:
        {
            cout << "Introduceti numele fisierului in care salvati raportul (fara .csv):";
            string raport;
            cin >> raport;
            raport = raport + ".csv";
            manager.raportareCSV(raport);
            break;
        }
        case 0:
            inapoi = true;
            break;
        default:
            cout << "\n!! Optiune invalida!";
            break;
        }
    }
}

void meniuSimulare(ServiceManager &manager)
{
    static int contorTicuri = 0;
    bool inapoi = false;

    while (!inapoi)
    {
        cout << "\n--- MODUL SIMULARE SERVICE ---";
        cout << "\n1. Executa PASUL URMATOR (1 Tic)";
        cout << "\n2. Executa AUTOMAT pana la finalizare";
        cout << "\n3. Resetare contor ticuri";
        cout << "\n0. Revenire la meniul principal";
        cout << "\nAlegerea dvs: ";

        int opt = citesteOptiune();
        switch (opt)
        {
        case 1:
            if (manager.getCereriAsteptareSize() == 0 && manager.getCereriActiveSize() == 0)
            {
                cout << "\n!! Nu exista cereri de procesat.";
            }
            else
            {
                manager.proceseazaUnTic(contorTicuri);
            }
            break;
        case 2:
            // Simulate ticks until completion
            manager.executaSimulareCompleta();
            break;
        case 3:
            contorTicuri = 0;
            cout << "\n>>> Contorul de timp a fost resetat la 0.";
            break;
        case 0:
            inapoi = true;
            break;
        default:
            cout << "\n!! Optiune invalida!";
            break;
        }
    }
}

int main()
{
    ServiceManager& manager = ServiceManager::getInstance();

    while (true)
    {
        cout << "\n================ FIX IT NOW - MENIU ================\n";
        cout << "1. GESTIUNE ANGAJATI (Module personal)\n";
        cout << "2. GESTIUNE CATALOG (Modele reparabile)\n";
        cout << "3. GESTIUNE CERERI (Input clienti)\n";
        cout << "4. MENIU SIMULARE SERVICE (Tic-toc)\n";
        cout << "5. GENERARE RAPOARTE (Statistici CSV)\n";
        cout << "0. IESIRE PROGRAM\n";
        cout << "========================================================\n";
        cout << "Alegerea dvs: ";

        int opt = citesteOptiune();
        if (opt == 0)
            {
                cout<<"PROGRAMUL S-A INCHEIAT. \n======VA MULTUMESC====== ";
                break;
            }

        switch (opt)
        {
        case 1:
            meniuAngajati(manager);
            break;
        case 2:
        {
            if (manager.EsteServiceFunctional())
                meniuCatalog(manager);
            else
                cout << "[ERROR]: Service infunctional. Prea putini angajati.";
            break;
        }
        case 3:
        {

            if (manager.EsteServiceFunctional())
                meniuCereri(manager);
            else
                cout << "[ERROR]: Service infunctional. Prea putini angajati.";
            break;
        }
        case 4:
        {
            if (manager.EsteServiceFunctional())
                meniuSimulare(manager);
            else
                cout << "[ERROR]: Service infunctional. Prea putini angajati.";

            break;
        }
        case 5:
        {
            if (manager.EsteServiceFunctional())
                meniuRaportari(manager);
            else
                cout << "[ERROR]: Service infunctional. Prea putini angajati.";

            break;
        }
        default:
            cout << "Optiune inexistenta!\n";
        }
    }
    return 0;
}