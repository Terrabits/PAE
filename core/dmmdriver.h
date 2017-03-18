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
Q_DECLARE_METATYPE(MeasurementType)

class DmmDriver
{
public:
    DmmDriver();
    DmmDriver(const QString &filename);

    QStringList     setupScpi;
    QString         setPointsScpi;
    QString         startScpi;
    double          sleepAfterStart_s;
    QString         queryDataScpi;
    MeasurementType measurementType;

    bool save(const QString &filename);

    // Measurement type
    static QString         toString(const MeasurementType &type);
    static MeasurementType toMeasurementType(const QString &string);

    bool operator=(const DmmDriver &other);
private:
    void init();
    bool open(const QString &filename);
};

bool operator==(const DmmDriver &left, const DmmDriver &right);
bool operator!=(const DmmDriver &left, const DmmDriver &right);

#endif // DMMDRIVER_H
