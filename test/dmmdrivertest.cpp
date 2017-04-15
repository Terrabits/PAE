#include "dmmdrivertest.h"

// Project
#include "dmmdriver.h"
// RsaToolbox
using namespace RsaToolbox;

// Qt
#include <QTest>

DmmDriverTest::DmmDriverTest(QObject *parent) :
    TestClass(parent)
{
    _src.setPath(SOURCE_DIR);
    _fixtures.setPath(_src.filePath("fixtures"));
    _temp.setPath(_src.filePath("temp"));
}

void DmmDriverTest::initTestCase() {
    _temp.removeRecursively();
    QVERIFY(_src.mkpath("temp"));
}

void DmmDriverTest::cleanupTestCase() {
    QVERIFY(_temp.removeRecursively());
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

    DmmDriver dmm(_src.filePath("fixtures/example_driver.json"));
    QCOMPARE(dmm.setupScpi, setupScpi);
    QCOMPARE(dmm.setPointsScpi, QString("TRIG:COUN %1"));
    QCOMPARE(dmm.startScpi, QString("INIT"));
    QCOMPARE(dmm.sleepAfterStart_s, 100.0e-3);
    QCOMPARE(dmm.queryDataScpi, QString("FETC?"));
    QCOMPARE(dmm.measurementType, MeasurementType::voltage);
}
void DmmDriverTest::save() {
    QString filename = _temp.filePath("dmm_driver_test_save.json");
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

    DmmDriver dmm_save;
    dmm_save.setupScpi         = setupScpi;
    dmm_save.setPointsScpi     = "TRIG:COUN %1";
    dmm_save.startScpi         = "INIT";
    dmm_save.sleepAfterStart_s = 100e-3;
    dmm_save.queryDataScpi     = "FETC?";
    dmm_save.measurementType   = MeasurementType::voltage;
    QVERIFY(dmm_save.save(filename));

    DmmDriver dmm_open(filename);
    QCOMPARE(dmm_open.setupScpi, dmm_save.setupScpi);
    QCOMPARE(dmm_open.setPointsScpi, dmm_save.setPointsScpi);
    QCOMPARE(dmm_open.startScpi, dmm_save.startScpi);
    QCOMPARE(dmm_open.sleepAfterStart_s, dmm_save.sleepAfterStart_s);
    QCOMPARE(dmm_open.queryDataScpi, dmm_save.queryDataScpi);
    QCOMPARE(dmm_open.measurementType, dmm_save.measurementType);

    QCOMPARE(dmm_open, dmm_save);
}
