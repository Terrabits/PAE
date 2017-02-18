#ifndef DMM_H
#define DMM_H

// Project
#include "dmmdriver.h"

// RsaToolbox
#include <GenericInstrument.h>

// Qt
#include <QScopedPointer>
#include <QString>

class Dmm
{
public:
    Dmm(const QString &resourceString, DmmDriver driver);

    bool isConnected();
    QString idString();

private:
    QString   _resource;
    DmmDriver _driver;
    QScopedPointer<RsaToolbox::GenericInstrument> _dmm;

};

#endif // DMM_H
