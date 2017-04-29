#include "dmmcontroller.h"


// RsaToolbox
using namespace RsaToolbox;


DmmController::DmmController()
{
    clear();
}

DmmController::~DmmController()
{

}

void DmmController::setSweepPoints(uint points) {
    _sweepPoints = points;
}
void DmmController::setPorts(const QVector<uint> &measuredPorts, uint inputPort) {
    _measuredPorts = measuredPorts;
    _inputPort = inputPort;
}

void DmmController::setStages(const QVector<StageSettings> &stages) {
    clear();
    _stages = stages;
    createDmms();
}

void DmmController::setup() {
    for (int i = 0; i < _stages.size(); i++) {
        Dmm &dmm = *_dmms[i];
        dmm.setup(_sweepPoints);
    }
}

void DmmController::start() {
    QSharedPointer<Dmm> _maxSleepDmm;
    uint _maxSleep = 0;
    for (int i = 0; i < _dmms.size(); i++) {
        QSharedPointer<Dmm> dmm = _dmms[i];
        dmm->start();
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
        if (!dmm.hasValidDriver()) {
            const QString filename = dmm.driver().filename();
            message = "*cannot parse \"%1\"";
            message = message.arg(filename);
            return false;
        }
    }
    return true;
}

void DmmController::createDmms() {
    _dmms.resize(_stages.size());
    for (int i = 0; i < _stages.size(); i++) {
        const StageSettings stage = _stages[i];
        _dmms[i].reset(new Dmm(stage.connectionType, stage.address, stage.driverPath));
    }
}

void DmmController::clear() {
    _sweepPoints = 0;
    _measuredPorts.clear();
    _inputPort   = 0;
    _stages.clear();
    _dmms.clear();
}

StageResult DmmController::readStage(uint i) const {
    Dmm &dmm = *_dmms[i];
    return StageResult(parse(dmm.readData()), _stages[i]);
}
QRowVector DmmController::parse(const QRowVector &rawData) const {
    const uint numPorts   = _measuredPorts.size();
    const uint inputIndex = _measuredPorts.indexOf(_inputPort);
    QRowVector result(_sweepPoints / numPorts);
    for (int i = 0; i < result.size(); i++) {
        result[i] = rawData[inputIndex + i * numPorts];
    }
    return result;
}
