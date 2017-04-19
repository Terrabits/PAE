#ifndef STAGESETTINGS_H
#define STAGESETTINGS_H


// RsaToolbox
#include <GenericBus.h>

class StageSettings
{
public:
    StageSettings();
    StageSettings(const QString &name);
    ~StageSettings();

    QString name;
    double powerSupply_V;
    double shuntResistor_ohms;
    RsaToolbox::ConnectionType connectionType;
    QString address;
    QString driverPath;

    QString connectionTypeString () const;
    QString driverFilename() const;

private:
    void init();
};

#endif // STAGESETTINGS_H
