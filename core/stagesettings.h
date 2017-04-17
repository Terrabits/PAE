#ifndef STAGESETTINGS_H
#define STAGESETTINGS_H


// RsaToolbox
#include <GenericBus.h>

class StageSettings
{
public:
    StageSettings();
    ~StageSettings();

    QString name;
    double powerSupply_V;
    double shuntResistor_ohms;
    RsaToolbox::ConnectionType connectionType;
    QString address;
    QString driverPath;

    QString connectionTypeString () const;
    QString driverFilename() const;
};

#endif // STAGESETTINGS_H
