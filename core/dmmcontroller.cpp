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

void DmmController::setStages(const QVector<StageSettings> &stages) {
    clear();
    _stages = stages;
}

bool DmmController::setup(uint points) {
    if (points <= 0) {
        QString msg = "Points must be >0";
        emit error(msg);
        return false;
    }
    _points = points;
    for (int i = 0; i < _dmms.size(); i++) {
        Dmm &dmm = *_dmms[i];
        if (!dmm->hasValidDriver()) {
            const QString filename = dmm->driver().filename();
            QString msg = "Cannot parse \"%1\"";
            msg = msg.arg(filename);
            emit error(msg);
            return false;
        }
        dmm.setup(_points);
    }
}

void DmmController::start() {
    for (int i = 0; i < _dmms.size(); i++) {
        Dmm &dmm = *_dmms[i];
        dmm.start();
    }
}

QMatrix2D DmmController::readData() {
    QMatrix2D results(_dmms.size());
    for (int i = 0; i < _dmms.size(); i++) {
        Dmm &dmm = *_dmms[i];
        results[i] = dmm.readData();
    }
    return result;
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
    _points = 0;
    _dmms.clear();
    _stages.clear();
}
