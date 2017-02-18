#include "dmmdrivertest.h"

// Project
#include "dmmdriver.h"
// RsaToolbox
using namespace RsaToolbox;

// Qt
#include <QTest>

DmmDriverTest::DmmDriverTest(QObject *parent) :
    TestClass(parent),
    _src(SOURCE_DIR)
{

}

void DmmDriverTest::open() {
    QStringList setupScpi;
    setupScpi << "SENS:VOLT:DC:RANG 100 mV"
              << "SENS:VOLT:DC:NPLC 0.001"
              << "TRIG:SOUR EXT"
              << "TRIG:DEL 10us"
              << "TRIG:SLOP POS"
              << "SAMP:SOUR IMM"
              << "SAMP:COUN 1"
              << "FORM:DATA ASC"
              << "FORM:BORD NORM";

    DmmDriver dmm(_src.filePath("fixtures/driver.json"));
    QCOMPARE(dmm.resourceString,  QString("TCPIP::192.168.1.11::INSTR"));
    QCOMPARE(dmm.supply_V,        50.0);
    QCOMPARE(dmm.measurementType, MeasurementType::voltage);
    QCOMPARE(dmm.resistor_Ohms,   5.0E-3);
    QCOMPARE(dmm.setupScpi,       setupScpi);
    QCOMPARE(dmm.pointsScpi,      QString("TRIG:COUN %1"));
    QCOMPARE(dmm.startScpi,       QString("INIT"));
    QCOMPARE(dmm.sleep_s,         100E-3);
    QCOMPARE(dmm.readScpi,        QString("FETC?"));
}
void DmmDriverTest::save() {
    DmmDriver dmm;
    dmm.resourceString  = "TCPIP::192.168.1.11::INSTR";
    dmm.supply_V        = 50.0;
    dmm.measurementType = MeasurementType::voltage;
    dmm.resistor_Ohms   = 5.0E-3;
    dmm.setupScpi << "SENS:VOLT:DC:RANG 100 mV"
                  << "SENS:VOLT:DC:NPLC 0.001"
                  << "TRIG:SOUR EXT"
                  << "TRIG:DEL 10us"
                  << "TRIG:SLOP POS"
                  << "SAMP:SOUR IMM"
                  << "SAMP:COUN 1"
                  << "FORM:DATA ASC"
                  << "FORM:BORD NORM";
    dmm.pointsScpi = "TRIG:COUN %1";
    dmm.startScpi  = "INIT";
    dmm.sleep_s    = 100.0E-3;
    dmm.readScpi   = "FETC?";
    QVERIFY(dmm.save(_src.filePath("output/driver.json")));

    DmmDriver saved_dmm(_src.filePath("output/driver.json"));
    QCOMPARE(dmm, saved_dmm);
}
