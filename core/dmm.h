#ifndef DMM_H
#define DMM_H

// Project
#include "dmmdriver.h"

// RsaToolbox
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

private:
    DmmDriver _driver;
};

#endif // DMM_H
