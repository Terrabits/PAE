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

class Dmm : public RsaToolbox::GenericInstrument
{
public:
    explicit Dmm(QObject *parent = 0);
    Dmm(RsaToolbox::GenericBus *bus, QObject *parent = 0);
    Dmm(RsaToolbox::ConnectionType type, QString address, QObject *parent = 0);
    Dmm(RsaToolbox::ConnectionType type, QString address, const DmmDriver &driver, QObject *parent = 0);

    DmmDriver driver() const;
    void setDriver(const DmmDriver &driver);
    void setDriver(const QString &filename);


    void setup(uint points);
    void start();
    RsaToolbox::QRowVector readData();

private:
    DmmDriver _driver;
    uint _points;

    void sendSetupScpi();
    void sendPointsScpi(uint points);
    void sendStartScpi();
    void sleep();
};

#endif // DMM_H
