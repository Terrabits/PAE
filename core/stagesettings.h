#ifndef STAGESETTINGS_H
#define STAGESETTINGS_H


// RsaToolbox
#include <GenericBus.h>

// Qt
#include <QDataStream>
#include <QString>

class StageSettings
{
public:
    StageSettings();
    StageSettings(const QString &name);
    ~StageSettings();

    QString name;

    bool hasAcceptableInput() const;
    bool hasAcceptableInput(QString &message) const;

    bool hasAcceptablePowerSupply() const;
    bool hasAcceptablePowerSupply(QString &message) const;
    double powerSupply_V;

    bool hasAcceptableShuntResistor() const;
    bool hasAcceptableShuntResistor(QString &message) const;
    double shuntResistor_ohms;

    bool hasAcceptableConnectionType() const;
    bool hasAcceptableConnectionType(QString &message) const;
    RsaToolbox::ConnectionType connectionType;
    QString connectionTypeString () const;

    bool hasAcceptableAddress() const;
    bool hasAcceptableAddress(QString &message) const;
    QString address;

    bool hasAcceptableDriverPath() const;
    bool hasAcceptableDriverPath(QString &message) const;
    QString driverPath;
    QString driverFilename() const;

private:
    void init();
};

QDataStream &operator<<(QDataStream &stream, const StageSettings &settings);
QDataStream &operator>>(QDataStream &stream, StageSettings &settings);

#endif // STAGESETTINGS_H
