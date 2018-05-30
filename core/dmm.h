#ifndef DMM_H
#define DMM_H

// Project
#include "dmmdriver.h"

// RsaToolbox
#include <Definitions.h>
#include <GenericInstrument.h>

// Qt
#include <QScopedPointer>
#include <QString>

// TODO:
// - update hasValidDriver()
// - add trigger delay setting

class Dmm : public RsaToolbox::GenericInstrument
{
public:
    explicit Dmm(QObject *parent = 0);
    Dmm(RsaToolbox::GenericBus *bus, QObject *parent = 0);
    Dmm(RsaToolbox::ConnectionType type, QString address, QObject *parent = 0);
    Dmm(RsaToolbox::ConnectionType type, QString address, const DmmDriver &driver, QObject *parent = 0);

    bool hasValidDriver() const;
    bool hasValidDriver(QString &message) const;
    DmmDriver driver() const;
    void setDriver(const DmmDriver &driver);
    void setDriver(const QString &filename);

    void setup(uint points, double triggerDelay_s);
    void start(bool sleep=true);
    void sleep();
    RsaToolbox::QRowVector readData(uint points);

private:
    DmmDriver _driver;
    uint _points;
    double _triggerDelay_s;

    void sendSetupScpi();
    void sendPointsScpi(uint points);
    void sendTriggerDelayScpi(double delay_s);
    void sendStartScpi();

};

#endif // DMM_H
