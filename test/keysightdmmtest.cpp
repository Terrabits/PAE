#include "keysightdmmtest.h"

// RsaToolbox
#include <Test.h>
using namespace RsaToolbox;

// Qt
#include <QDebug>

KeysightDmmTest::KeysightDmmTest(ConnectionType vnaType, const QString &vnaAddress, ConnectionType dmmType, const QString &dmmAddress, QObject *parent) :
    TestClass(parent),
    _vnaType   (vnaType   ),
    _vnaAddress(vnaAddress),
    _dmmType   (dmmType   ),
    _dmmAddress(dmmAddress)
{

}
KeysightDmmTest::~KeysightDmmTest()
{
    _dmm.reset();
}

// Once
void KeysightDmmTest::initTestCase() {

}
void KeysightDmmTest::cleanupTestCase() {

}

// Per test
void KeysightDmmTest::init() {
    _vna.reset(new Vna(_vnaType, _vnaAddress));
    QVERIFY(_vna->isConnected());
    QVERIFY(!_vna->idString().isEmpty());

    _dmm.reset(new KeysightDmm(_dmmType, _dmmAddress));
    QVERIFY(_dmm->isConnected());
    _dmm->preset();
    _dmm->pause(10000);
    _dmm->clearStatus();
}
void KeysightDmmTest::cleanup() {
    _dmm.reset();
}

// Tests
void KeysightDmmTest::idString() {
    QVERIFY(!_dmm->idString().trimmed().isEmpty());
}
void KeysightDmmTest::settings() {
    _dmm->setRange("100 mV");
    _dmm->setNplc(0.001);
    _dmm->setTriggerSource("EXT");
    _dmm->setTriggerDelay(20E-6);
    _dmm->setTriggerCount(1);
    _dmm->setTriggerSlope("POS");
    _dmm->setSampleSource("IMM");
    _dmm->setSampleCount(1);
    _dmm->setDataFormat("ASC");
    _dmm->setBitOrder("NORM");

    QVERIFY(!_dmm->isError());
}

void KeysightDmmTest::measure() {
    const uint points = 101;

    _dmm->setRange("100 mV");
    _dmm->setNplc(0.001);
    _dmm->setTriggerSource("EXT");
    _dmm->setTriggerDelay(20E-6);
    _dmm->setTriggerCount(points);
    _dmm->setTriggerSlope("POS");
    _dmm->setSampleSource("IMM");
    _dmm->setSampleCount(1);
    _dmm->setDataFormat("ASC");
    QVERIFY(!_dmm->isError());

    _dmm->startMeasurement();
    QVERIFY(!_dmm->isError());

    _vna->startSweeps();

    QRowVector results = _dmm->results(points);
    QCOMPARE(results.size(), int(points));
}
