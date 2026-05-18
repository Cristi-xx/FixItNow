#pragma once
#include <memory>
class Tehnician;
using namespace std;
/**
 * Interface for salary calculation.
 * Follows the Single Responsibility principle.
 */
class ICalculSalariu {
public:
    virtual double getValoare() const = 0;
    virtual ~ICalculSalariu() = default;
};

/**
 * Fixed salary: 4000 RON
 */
class SalariuDeBaza : public ICalculSalariu {
public:
    double getValoare() const override;
};

/**
 * Abstract decorator for the bonus chain.
 */
class Decorator_Salariu : public ICalculSalariu {
protected:
    shared_ptr<ICalculSalariu> componenta;

public:
    explicit Decorator_Salariu(shared_ptr<ICalculSalariu> comp);
    double getValoare() const override;
};

// Concrete bonuses

class BonusFidelitate : public Decorator_Salariu {
private:
    int ani_vechime;
public:
    BonusFidelitate(shared_ptr<ICalculSalariu> comp, int ani);
    double getValoare() const override;
};

class BonusTransport : public Decorator_Salariu {
public:
    explicit BonusTransport(shared_ptr<ICalculSalariu> comp);
    double getValoare() const override;
};

class BonusTehnician : public Decorator_Salariu {
private:
    shared_ptr<Tehnician> tehnician; // Live link - to get the live value of repairs
public:
    BonusTehnician(shared_ptr<ICalculSalariu> comp, shared_ptr<Tehnician> teh);
    double getValoare() const override;
};
class SporConducere : public Decorator_Salariu {
public:
    explicit SporConducere(shared_ptr<ICalculSalariu> comp);
    double getValoare() const override;
};

