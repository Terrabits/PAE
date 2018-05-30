#include "dmmcontroller.h"

// Project
#include "settings.h"

// RsaToolbox
using namespace RsaToolbox;

// Qt
#include <QDebug>
#include <QFileInfo>

// stdlib
#include <cassert>


DmmController::DmmController()
{
    clear();
}
DmmController::DmmController(const DmmController &other) {
    copy(other);
}

DmmController::~DmmController()
{

}

void DmmController::setSweepPoints(uint points) {
    _sweepPoints = points;
}
void DmmController::setTriggerDelay(double delay_s) {
    if (delay_s < 0) {
        _triggerDelay_s = 0;
    }
    else {
        _triggerDelay_s = delay_s;
    }
}

void DmmController::setPorts(const QVector<uint> &measuredPorts, uint inputPort) {
    _measuredPorts = measuredPorts;
    _inputPort = inputPort;
}
void DmmController::addStage(StageSettings settings) {
    _stages.append(settings);
    createDmms();
}

void DmmController::setStages(const QVector<StageSettings> &stages) {
    clear();
    _stages = stages;
    createDmms();
}

void DmmController::setup() {
    for (int i = 0; i < _stages.size(); i++) {
        Dmm &dmm = *_dmms[i];
        dmm.setup(_sweepPoints * _measuredPorts.size(), _triggerDelay_s);
    }
}

void DmmController::start() {
    QSharedPointer<Dmm> _maxSleepDmm;
    uint _maxSleep = 0;
    for (int i = 0; i < _dmms.size(); i++) {
        QSharedPointer<Dmm> dmm = _dmms[i];
        dmm->start(false);
        if (dmm->driver().sleepAfterStart_s > _maxSleep) {
            _maxSleepDmm = dmm;
        }
    }
    _maxSleepDmm->sleep();
}

QVector<StageResult> DmmController::readResults() const {
    QVector<StageResult> results(_dmms.size());
    for (int i = 0; i < _dmms.size(); i++) {
        results[i] = readStage(i);
    }
    return results;
}

DmmController &DmmController::operator=(const DmmController &other) {
    copy(other);
    return *this;
}

bool DmmController::isConnected(QString &message) const {
    message.clear();
    if (_dmms.isEmpty()) {
        return false;
    }

    for (int i = 0; i < _dmms.size(); i++) {
        if (!_dmms[i]->isConnected() || _dmms[i]->idString().isEmpty()) {
            StageSettings stage = _stages[i];
            message = "*could not connect to DMM at %1 %2";
            message = message.arg(stage.connectionTypeString());
            message = message.arg(stage.address);
            return false;
        }
    }
    return true;
}

bool DmmController::hasAcceptableInput() {
    QString msg;
    return hasAcceptableInput(msg);
}
bool DmmController::hasAcceptableInput(QString &message) {
    message.clear();
    if (!hasAcceptableStageInput(message)) {
        return false;
    }

    return true;
}

bool DmmController::hasAcceptableStageInput() {
    QString msg;
    return hasAcceptableStageInput(msg);
}
bool DmmController::hasAcceptableStageInput(QString &message) {
    message.clear();
    if (_stages.isEmpty() || _dmms.isEmpty()) {
        message = "*must have at least one stage/DMM";
        return false;
    }
    for (int i = 0; i < _dmms.size(); i++) {
        StageSettings stage = _stages[i];
        if (!stage.hasAcceptableInput(message)) {
            return false;
        }

        Dmm &dmm = *_dmms[i];
        QString driverMsg;
        if (!dmm.hasValidDriver(driverMsg)) {
            QString filename = dmm.driver().filename();
            filename = QFileInfo(filename).fileName();
            message = "*driver \"%1\": %2";
            message = message.arg(filename);
            message = message.arg(driverMsg);
            return false;
        }
    }
    return true;
}

void DmmController::createDmms() {
    _dmms.resize(_stages.size());
    _logs.resize(_stages.size());
    const QString filename = "Stage %1 log.txt";
    for (int i = 0; i < _stages.size(); i++) {
        const StageSettings stage = _stages[i];
        QString filename = "Stage %1 log.txt";
        _logs[i].reset(new Log(dataDir.filePath(filename.arg(i+1)), APP_NAME, APP_VERSION));
        _logs[i]->printHeader();
        _dmms[i].reset(new Dmm(stage.connectionType, stage.address, stage.driverPath));
        _dmms[i]->useLog(_logs[i].data());
        _dmms[i]->printInfo();
    }
}

void DmmController::clear() {
    _sweepPoints    = 0;
    _triggerDelay_s = 0;
    _measuredPorts   .clear();
    _inputPort      = 0;
    _stages          .clear();
    _dmms            .clear();
    _logs            .clear();
}

StageResult DmmController::readStage(uint i) const {
    const uint dmmPoints = _sweepPoints * _measuredPorts.size();
    Dmm &dmm = *_dmms[i];
    QRowVector result = dmm.readData(dmmPoints);
    if (result.size() != dmmPoints) {
         return StageResult(QRowVector(), _stages[i]);
    }
    else {
        return StageResult(parse(result), _stages[i]);
    }
}
QRowVector DmmController::parse(const QRowVector &rawData) const {
    const uint numPorts   = _measuredPorts.size();
    const uint inputIndex = _measuredPorts.indexOf(_inputPort);
    QRowVector result(_sweepPoints);
    for (int i = 0; i < result.size(); i++) {
        result[i] = rawData[i * numPorts + inputIndex];
    }
    return result;
}

void DmmController::copy(const DmmController &other) {
    _sweepPoints   = other._sweepPoints;
    _measuredPorts = other._measuredPorts;
    _inputPort     = other._inputPort;
    _dmms          = other._dmms;
}
