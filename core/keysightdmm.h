#ifndef KEYSIGHTDMM_H
#define KEYSIGHTDMM_H

// RsaToolbox
#include <GenericInstrument.h>

// Qt
#include <QObject>
#include <QString>
#include <QStringList>

class KeysightDmm : public RsaToolbox::GenericInstrument
{
    Q_OBJECT

public:
    explicit KeysightDmm(QObject *parent = 0);
    KeysightDmm(RsaToolbox::GenericBus *bus, QObject *parent = 0);
    KeysightDmm(RsaToolbox::ConnectionType type, QString address, QObject *parent = 0);

    void setRange(const QString &value);
    void setNplc(double value);

    void setTriggerSource(const QString &value);
    void setTriggerDelay(double value);
    void setTriggerCount(uint value);
    void setTriggerSlope(const QString &value);

    void setSampleSource(const QString &value);
    void setSampleCount(uint value);

    void setDataFormat(const QString &value);
    void setBitOrder(const QString &value);

    void startMeasurement();
    void abort();
    RsaToolbox::QRowVector results(uint points);

    bool isError  ();
    bool nextError(QString     &message);
    bool errors   (QStringList &messages);

};

#endif // KEYSIGHTDMM_H
