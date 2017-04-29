#ifndef DMMCONTROLLER_H
#define DMMCONTROLLER_H


// Project
#include "dmm.h"
#include "stageresult.h"
#include "stagesettings.h"

// RsaToolbox
#include <Definitions.h>

// Qt
#include <QSharedPointer>
#include <QString>
#include <QVector>


class DmmController
{
public:
    DmmController();
    ~DmmController();

    bool isConnected(QString &message) const;

    bool hasAcceptableInput();
    bool hasAcceptableInput(QString &message);

    bool hasAcceptableStageInput();
    bool hasAcceptableStageInput(QString &message);
    void setStages(const QVector<StageSettings> &stages);

    void setSweepPoints(uint points);
    void setPorts(const QVector<uint> &measuredPorts, uint inputPort);

    void setup();
    void start();
    QVector<StageResult> readResults() const;

private:
    uint _sweepPoints;
    QVector<uint> _measuredPorts;
    uint          _inputPort;
    QVector<StageSettings> _stages;

    void createDmms();
    QVector<QSharedPointer<Dmm>> _dmms;

    void clear();
    StageResult readStage(uint i) const;
    RsaToolbox::QRowVector parse(const RsaToolbox::QRowVector &rawData) const;
};

#endif // DMMCONTROLLER_H
