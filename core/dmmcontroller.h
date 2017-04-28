#ifndef DMMCONTROLLER_H
#define DMMCONTROLLER_H


// Project
#include "dmm.h"
#include "stageresult.h"
#include "stagesettings.h"

// RsaToolbox
#include <Definitions.h>

// Qt
#include <QObject>
#include <QSharedPointer>
#include <QString>
#include <QVector>


class DmmController : public QObject
{
    Q_OBJECT
public:
    explicit DmmController(QObject *parent = 0);
    ~DmmController();

    void setSweepPoints(uint points);
    void setPorts(const QVector<uint> &measuredPorts, uint inputPort);
    void setStages(const QVector<StageSettings> &stages);

    bool isConnected() const;
    QVector<StageResult> readResults();

signals:
    void error(const QString &message);

public slots:
    bool connect();
    bool setup();
    void start();

private:
    bool _isConnected;
    uint _sweepPoints;
    QVector<uint> _measuredPorts;
    uint          _inputPort;
    QVector<StageSettings>       _stages;
    QVector<QSharedPointer<Dmm>> _dmms;

    bool _setup(uint i);
    void clear();
    StageResult readStage(uint i);
    RsaToolbox::QRowVector parse(const RsaToolbox::QRowVector &rawData);
};

#endif // DMMCONTROLLER_H
