#include "../Electrocasnic.h"

class Frigider : public Electrocasnic {
private:
    bool areCongelator;

public:
    Frigider(string marca, string model, int an, double pret, bool congelator);
    string descriere() const override;
};