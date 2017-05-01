#include "dmm.h"

// RsaToolbox
#include <General.h>
using namespace RsaToolbox;

// Qt
#include <QFileInfo>
#include <QThread>


Dmm::Dmm(QObject *parent) :
    GenericInstrument(parent)
{

}
Dmm::Dmm(GenericBus *bus, QObject *parent) :
    GenericInstrument(bus, parent)
{

}
Dmm::Dmm(ConnectionType type, QString address, QObject *parent) :
    GenericInstrument(type, address, parent)
{

}

Dmm::Dmm(ConnectionType type, QString address, const DmmDriver &driver, QObject *parent) :
    GenericInstrument(type, address, parent),
    _driver(driver)
{

}

bool Dmm::hasValidDriver() const {
    return _driver.isOpen();
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

void Dmm::setup(uint points) {
    _points = points;
    sendSetupScpi();
    sendPointsScpi(points);
}
void Dmm::start() {
    sendStartScpi();
//    sleep();
}
QRowVector Dmm::readData(uint points) {
    const uint bufferSize_B = points * 25 + 1000;
    return parseDoubles(query(_driver.queryDataScpi, bufferSize_B), ",");
}

void Dmm::sendSetupScpi() {
    for (int i = 0; i < _driver.setupScpi.size(); i++) {
        write(_driver.setupScpi[i]);
    }
}
void Dmm::sendPointsScpi(uint points) {
    write(_driver.setPointsScpi.arg(points));
}
void Dmm::sendStartScpi() {
    write(_driver.startScpi);
}
void Dmm::sleep() {
    QThread::sleep(_driver.sleepAfterStart_s);
}
