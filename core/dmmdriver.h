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

enum /*class*/ DataFormat {
    Ascii,
    Binary32Bit,
    Binary64Bit
};
Q_DECLARE_METATYPE(DataFormat)

class DmmDriver
{
public:
    DmmDriver();
    DmmDriver(const DmmDriver &other);
    DmmDriver(const QString &filename);

    bool isOpen() const;
    bool isValid() const;
    bool isValid(QString &errorMessage) const;
    QString filename() const;

    QString         driverVersion;
    bool            preset;
    QStringList     setupScpi;
    QString         setPointsScpi;
    QString         setTriggerDelayScpi;
    QString         startScpi;
    double          sleepAfterStart_s;
    DataFormat      dataFormat;
    QString         queryDataScpi;
    MeasurementType measurementType;

    uint majorVersion() const;
    uint minorVersion() const;
    uint bugVersion  () const;

    bool save(const QString &filename);

    // Measurement type
    static QString         toString         (const MeasurementType &type);
    static MeasurementType toMeasurementType(QString string, bool *ok = 0);

    // Data format
    static QString    toString    (const DataFormat &format);
    static DataFormat toDataFormat(      QString     string, bool *ok = 0);

    void operator=(const DmmDriver &other);
private:
    QString _filename;
    bool    _isOpen;
    bool    isOpenError() const;
    QString _openError;

    void init();
    void clearKeys();
    bool open(const QString &filename);

    // validation
    bool validDriverVersion  () const;
    bool validSetPointsScpi  () const;
    bool validSetTriggerDelayScpi() const;
    bool validStartScpi      () const;
    bool validSleepAfterStart() const;
    bool validQueryDataScpi  () const;

    void copy(const DmmDriver &other);
};

bool operator==(const DmmDriver &left, const DmmDriver &right);
bool operator!=(const DmmDriver &left, const DmmDriver &right);

#endif // DMMDRIVER_H
