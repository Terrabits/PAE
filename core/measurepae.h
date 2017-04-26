#ifndef MEASUREPAE_H
#define MEASUREPAE_H


// Project
#include "dmmcontroller.h"
#include "stageresult.h"


// Qt
#include <QObject>


class MeasurePAE : public QObject
{
    Q_OBJECT
public:
    explicit MeasurePAE(QObject *parent = 0);
    ~MeasurePAE();

    void setSweepPoints(uint points);
    void setPorts(const QVector<uint> &measuredPorts, uint inputPort);
    void setStages(const QVector<StageSettings> &stages);

    QVector<StageResult> stageResults();
    RsaToolbox::QRowVector dcPower_W ();
    RsaToolbox::QRowVector pae_pct();
    RsaToolbox::QRowVector de_pct ();

signals:
    void error(const QString &message);

public slots:
    bool start();

private:
    DmmController _controller;
};

#endif // MEASUREPAE_H
