#include <iostream>
#include <vector>
#include <memory>
#include "../Electrocasnice/Electrocasnic.h"
#include <ctime>
#include "../Muncitori/Angajatii/Tehnician.h"

class CereriReparatie {
private:
    static int ContorID;
    int id;
    time_t timestamp;           // "request submission timestamp"
    int complexitate;           // "level 1-5"
    int durataEstimata;         // "estimated duration"
    double pretTotal;           // "repair price"
    
    int durataInitiala;
    unique_ptr<Electrocasnic> aparat; // Owns the appliance
    shared_ptr<Tehnician> tehnicianAsignat;
public:
    CereriReparatie( unique_ptr<Electrocasnic>, int, time_t );

    void proceseazaTic();       // Decrease duration
    bool esteFinalizata() const;
    void afiseazaStatus() const;
    
    
    // Getteri
    int getId() const;
    double getPret() const;
    time_t getTimestamp() const;
    int getDurata() const;
    int getDurataInitiala() const;
    int getVechime() const;
    string getTipAparat() const;
    string getMarcaAparat() const;
    string getModelAparat() const;

    void setTehnicianAsignat(std::shared_ptr<Tehnician> t);
    std::shared_ptr<Tehnician> getTehnicianAsignat();
};