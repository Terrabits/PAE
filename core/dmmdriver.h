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
    DmmDriver(const DmmDriver &other);
    DmmDriver(const QString &filename);

    bool isOpen() const;
    QString filename() const;

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

    void operator=(const DmmDriver &other);
private:
    bool _isOpen;
    QString _filename;
    void init();
    bool open(const QString &filename);

    void copy(const DmmDriver &other);
};

bool operator==(const DmmDriver &left, const DmmDriver &right);
bool operator!=(const DmmDriver &left, const DmmDriver &right);

#endif // DMMDRIVER_H
