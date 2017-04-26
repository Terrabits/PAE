#include "dmmcontroller.h"


// RsaToolbox
using namespace RsaToolbox;


DmmController::DmmController(QObject *parent) : QObject(parent)
{
    clear();
}

DmmController::~DmmController()
{

}

void DmmController::setSweepPoints(uint points) {
    if (points <= 0) {
        QString msg = "Points must be >0";
        emit error(msg);
        return;
    }

    _sweepPoints = points;
}
void DmmController::setPorts(const QVector<uint> &measuredPorts, uint inputPort) {
    if (measuredPorts.isEmpty()) {
        emit error("measured ports cannot be empty");
        return;
    }
    if (measuredPorts.contains(0)) {
        emit error("Ports must be >0");
        return;
    }
    if (inputPort == 0) {
        emit error("Ports must be >0");
        return;
    }
    if (!measuredPorts.contains(inputPort)) {
        emit error("Cannot find input port in measured ports");
        return;
    }

    _measuredPorts = measuredPorts;
    _inputPort = inputPort;
}

void DmmController::setStages(const QVector<StageSettings> &stages) {
    clear();
    _stages = stages;
}

bool DmmController::setup() {
    if (_sweepPoints <= 0) {
        QString msg = "Points must be >0";
        emit error(msg);
        return false;
    }
    for (int i = 0; i < _dmms.size(); i++) {
        Dmm &dmm = *_dmms[i];
        if (!dmm->hasValidDriver()) {
            const QString filename = dmm->driver().filename();
            QString msg = "Cannot parse \"%1\"";
            msg = msg.arg(filename);
            emit error(msg);
            return false;
        }
        dmm.setup(_sweepPoints);
    }
}

void DmmController::start() {
    for (int i = 0; i < _dmms.size(); i++) {
        Dmm &dmm = *_dmms[i];
        dmm.start();
    }
}

QVector<StageResult> DmmController::readResults() {
    QVector<StageResult> results(_dmms.size());
    for (int i = 0; i < _dmms.size(); i++) {
        results[i] = readStage(i);
    }
    return results;
}

bool DmmController::isConnected() const {
    return _isConnected;
}

bool DmmController::connect() {
    for (int i = 0; i < _stages.size(); i++) {
        const StageSettings stage = _stages[i];
        _dmms[i].reset(new Dmm(stage.connectionType, stage.address, stage.driverPath));
        if (!_dmms[i]->isConnected() || _dmms[i]->idString().isEmpty()) {
            QString msg = "Could not connect to DMM at %1 %2";
            msg = msg.arg(stage.connectionTypeString());
            msg = msg.arg(stage.address);
            _isConnected = false;
            emit error(msg);
            return false;
        }
    }
    _isConnected = true;
    return true;
}

void DmmController::clear() {
    _isConnected = false;
    _sweepPoints = 0;
    _measuredPorts.clear();
    _inputPort   = 0;
    _dmms.clear();
    _stages.clear();
}

StageResult DmmController::readStage(uint i) {
    Dmm &dmm = *_dmms[i];
    return StageResult(parse(dmm.readData()), _stages[i]);
}
QRowVector DmmController::parse(const QRowVector &rawData) {
    const uint numPorts   = _measuredPorts.size();
    const uint inputIndex = _measuredPorts.indexOf(_inputPort);
    QRowVector result(_sweepPoints / numPorts);
    for (int i = 0; i < result.size(); i++) {
        result[i] = rawData[inputIndex + i * numPorts];
    }
    return result;
}
