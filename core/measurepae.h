#ifndef MEASUREPAE_H
#define MEASUREPAE_H


// Project
#include "dmmcontroller.h"
#include "stageresult.h"

// RsaToolbox
#include <Vna.h>

// Qt
#include <QObject>
#include <QString>

class MeasurePaeTest;

class MeasurePAE : public QObject
{
    friend class MeasurePaeTest;

    Q_OBJECT
public:
    enum /*class*/ Calculation {
        powerAddedEfficiency = 0,
        drainEfficiency      = 1
    };

    explicit MeasurePAE(QObject *parent = 0);
    ~MeasurePAE();

    void setVna(RsaToolbox::Vna *vna);

    bool hasAcceptableInput(QString &message);

    void setCalculation(Calculation calculation);

    bool hasAcceptableTraceInput(QString &message);
    void setInputTrace (const QString &name);
    void setGainTrace  (const QString &name);
    void setOutputTrace(const QString &name);

    bool isOneChannel() const;
    uint channel() const;

    uint sweepPoints() const;
    QVector<uint> sourcePorts() const;

    bool hasAcceptableStageInput(QString &message);
    void setStages(const QVector<StageSettings> &stages);

    QVector<StageResult>   stageResults  () const;
    RsaToolbox::QRowVector dcPower_W     () const;
    RsaToolbox::QRowVector efficiency_pct() const;

signals:
    void error(const QString &message);

public slots:
    void run();

private:
    void sweepVna();
    mutable RsaToolbox::Vna *_vna;

    bool isPowerAddedEfficiency() const;
    bool isDrainEfficiency     () const;
    Calculation _calculation;

    bool isInputTrace () const;
    bool isGainTrace  () const;
    bool isOutputTrace() const;
    QString _inputTrace;
    QString _gainTrace;
    QString _outputTrace;

    uint inputTraceChannel () const;
    uint gainTraceChannel  () const;
    uint outputTraceChannel() const;
    uint    _channel;

    QVector<StageSettings> _stages;
    DmmController          _controller;

    RsaToolbox::QRowVector pin_W () const;
    RsaToolbox::QRowVector pout_W() const;
    RsaToolbox::QRowVector readPin_W () const;
    RsaToolbox::QRowVector readGain_U() const;
    RsaToolbox::QRowVector readPout_W() const;
    RsaToolbox::QRowVector read_W(const QString &name) const;
    RsaToolbox::QRowVector calculatePin_W () const;
    RsaToolbox::QRowVector calculatePout_W() const;

    mutable QVector<StageResult> _results;

};

#endif // MEASUREPAE_H
