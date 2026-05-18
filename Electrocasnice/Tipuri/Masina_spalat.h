#include "../Electrocasnic.h"

class MasinaSpalat : public Electrocasnic {
private:
    double capacitateKg;

public:
    MasinaSpalat(string marca, string model, int an, double pret, double cap);
    string descriere() const override;
};