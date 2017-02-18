#ifndef DMMDRIVER_H
#define DMMDRIVER_H

// Qt
#include <QMetaType>
#include <QString>
#include <QStringList>

enum /*class*/ MeasurementType {
    voltage,
    current
};

class DmmDriver
{
public:
    DmmDriver();
    DmmDriver(const QString &filename);

    QString         resourceString;
    double          supply_V;
    MeasurementType measurementType;
    double          resistor_Ohms;
    QStringList     setupScpi;
    QString         pointsScpi;
    QString         startScpi;
    double          sleep_s;
    QString         readScpi;

    bool save(const QString &filename);

private:
    void init();
    bool open(const QString &filename);
};

bool operator==(const DmmDriver &left, const DmmDriver &right);
bool operator!=(const DmmDriver &left, const DmmDriver &right);

#endif // DMMDRIVER_H
