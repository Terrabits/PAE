#include "dmm.h"

// RsaToolbox
#include <General.h>
using namespace RsaToolbox;

// Qt
#include <QDebug>
#include <QFileInfo>
#include <QThread>


Dmm::Dmm(QObject *parent) :
    GenericInstrument(parent),
    _points(0),
    _triggerDelay_s(0)
{

}
Dmm::Dmm(GenericBus *bus, QObject *parent) :
    GenericInstrument(bus, parent),
    _points(0),
    _triggerDelay_s(0)
{

}
Dmm::Dmm(ConnectionType type, QString address, QObject *parent) :
    GenericInstrument(type, address, parent),
    _points(0),
    _triggerDelay_s(0)
{

}

Dmm::Dmm(ConnectionType type, QString address, const DmmDriver &driver, QObject *parent) :
    GenericInstrument(type, address, parent),
    _driver(driver),
    _points(0),
    _triggerDelay_s(0)
{

}

bool Dmm::hasValidDriver() const {
    QString msg;
    return _driver.isValid(msg);
}
bool Dmm::hasValidDriver(QString &message) const {
    return _driver.isValid(message);
}
DmmDriver Dmm::driver() const {
    return _driver;
}
void Dmm::setDriver(const DmmDriver &driver) {
    _driver = driver;
}
void Dmm::setDriver(const QString &filename) {
    _driver = DmmDriver(filename);
}

void Dmm::setup(uint points, double triggerDelay_s) {
    _points = points;
    _triggerDelay_s = triggerDelay_s;

    if (_driver.preset) {
        preset();
        pause(5000);
    }
    sendSetupScpi();
    sendPointsScpi(points);
    sendTriggerDelayScpi(triggerDelay_s);
    pause();
}
void Dmm::start(bool sleep) {
    sendStartScpi();
    if (sleep) {
        this->sleep();
    }
}
QRowVector Dmm::readData(uint points) {
    const uint bufferSize_B = points * 25 + 1000;
    const QRowVector data = parseDoubles(query(_driver.queryDataScpi, bufferSize_B), ",");
    if (data.size() != points) {
        return QRowVector();
    }
    else {
        return data;
    }
}

void Dmm::sendSetupScpi() {
    for (int i = 0; i < _driver.setupScpi.size(); i++) {
        write(_driver.setupScpi[i]);
    }
}
void Dmm::sendPointsScpi(uint points) {
    write(_driver.setPointsScpi.arg(points));
}
void Dmm::sendTriggerDelayScpi(double delay_s) {
    write(_driver.setTriggerDelayScpi.arg(delay_s));
}

void Dmm::sendStartScpi() {
    write(_driver.startScpi);
}
void Dmm::sleep() {
    QThread::sleep(_driver.sleepAfterStart_s);
}
