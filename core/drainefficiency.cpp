#include "drainefficiency.h"

// RsaToolbox
#include <General.h>
using namespace RsaToolbox;

// Qt
#include <QDebug>

// stdlib
#include <cmath>

DrainEfficiency::DrainEfficiency()
{

}

void DrainEfficiency::setR(double Ohms) {
    _r_ohms = Ohms;
}
void DrainEfficiency::setVdd(double volts) {
    _vdd_V = volts;
}

QRowVector DrainEfficiency::calculate(QRowVector supply_V, QRowVector pout_dBm) {
    QRowVector supply_W = multiply(supply_V, (_vdd_V / _r_ohms));
    QRowVector pout_W   = toW(pout_dBm);

    qDebug(QString("supply: %1").arg(supply_W.first()).toUtf8());
    qDebug(QString("Pout:   %1").arg(pout_W.first()).toUtf8());

    return divideEach(pout_W, supply_W);
}
QRowVector DrainEfficiency::calculatePercent(QRowVector supply_V, QRowVector pout_dBm) {
    return multiply(calculate(supply_V, pout_dBm), 100.0);
}

QRowVector DrainEfficiency::toW(QRowVector dBm) {
    QRowVector watts;
    watts.resize(dBm.size());
    for (int i = 0; i < dBm.size(); i++) {
        watts[i] = pow(10.0, dBm[i] / 10.0) / 1000.0;
    }
    return watts;
}
