#ifndef DRAINEFFICIENCY_H
#define DRAINEFFICIENCY_H

// RsaToolbox
#include <Definitions.h>

class DrainEfficiency
{
public:
    DrainEfficiency();

    void setR(double Ohms);
    void setVdd(double volts);

    RsaToolbox::QRowVector calculate(RsaToolbox::QRowVector supply_V, RsaToolbox::QRowVector pout_dBm);
    RsaToolbox::QRowVector calculatePercent(RsaToolbox::QRowVector supply_V, RsaToolbox::QRowVector pout_dBm);

private:
    double _r_ohms;
    double _vdd_V;

    static RsaToolbox::QRowVector toW(RsaToolbox::QRowVector dBm);
};

#endif // DRAINEFFICIENCY_H
