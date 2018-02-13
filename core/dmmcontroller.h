#ifndef DMMCONTROLLER_H
#define DMMCONTROLLER_H


// Project
#include "dmm.h"
#include "stageresult.h"
#include "stagesettings.h"
class DmmControllerTest;

// RsaToolbox
#include <Definitions.h>

// Qt
#include <QSharedPointer>
#include <QString>
#include <QVector>

class DmmControllerTest;
class MeasurePaeTest;

class DmmController
{
    friend class DmmControllerTest;
    friend class MeasurePaeTest;

public:
    DmmController();
    DmmController(const DmmController &other);
    ~DmmController();

    bool isConnected(QString &message) const;

    bool hasAcceptableInput();
    bool hasAcceptableInput(QString &message);

    bool hasAcceptableStageInput();
    bool hasAcceptableStageInput(QString &message);
    void addStage(StageSettings settings);
    void setStages(const QVector<StageSettings> &stages);

    void setSweepPoints(uint points);
    void setPorts(const QVector<uint> &measuredPorts, uint inputPort);

    void setup();
    void start();
    QVector<StageResult> readResults() const;

    DmmController &operator=(const DmmController &other);

private:
    uint _sweepPoints;
    QVector<uint> _measuredPorts;
    uint          _inputPort;
    QVector<StageSettings> _stages;

    void createDmms();
    QVector<QSharedPointer<Dmm>> _dmms;
    QVector<QSharedPointer<RsaToolbox::Log>> _logs;

    void clear();
    StageResult readStage(uint i) const;
    RsaToolbox::QRowVector parse(const RsaToolbox::QRowVector &rawData) const;

    void copy(const DmmController &other);
};

#endif // DMMCONTROLLER_H
