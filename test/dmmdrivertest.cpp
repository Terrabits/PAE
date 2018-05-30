#include "dmmdrivertest.h"

// Project
#include "dmmdriver.h"

// RsaToolbox
using namespace RsaToolbox;

// Qt
#include <QString>
#include <QTest>

DmmDriverTest::DmmDriverTest(QObject *parent) :
    TestClass(parent)
{
    _src.setPath(SOURCE_DIR);
    _fixtures.setPath(_src.filePath("fixtures"));
    _dmmdrivertestFixtures.setPath(_fixtures.filePath("dmmdrivertest"));
    _temp.setPath(_src.filePath("temp"));
}

void DmmDriverTest::initTestCase() {
    int i = 0;
    while (_temp.exists() && i < 200) {
        _temp.removeRecursively();
        i++;
    }

    i = 0;
    while (!_temp.exists() && i < 200) {
        _src.mkpath("temp");
        i++;
    }
    QVERIFY(_temp.exists());
}

void DmmDriverTest::cleanupTestCase() {
    QVERIFY(_temp.removeRecursively());
}

void DmmDriverTest::openTest() {
    QStringList setupScpi;
    setupScpi << "SENS:VOLT:DC:RANG 100 mV"
              << "SENS:VOLT:DC:NPLC 0.001"
              << "TRIG:SOUR EXT"
              << "TRIG:SLOP POS"
              << "SAMP:SOUR IMM"
              << "SAMP:COUN 1"
              << "FORM:DATA ASC"
              << "FORM:BORD NORM";

    DmmDriver dmm(_dmmdrivertestFixtures.filePath("valid-driver.json"));
    QVERIFY(dmm.isOpen());

    QString errorMsg = "This text should be cleared";
    QVERIFY(dmm.isValid(errorMsg));
    QVERIFY(errorMsg.isEmpty());

    QCOMPARE(dmm.driverVersion, QString("2.0.0"));
    QCOMPARE(dmm.majorVersion(),   uint( 2     ));
    QCOMPARE(dmm.minorVersion(),   uint(   0   ));
    QCOMPARE(dmm.bugVersion  (),   uint(     0 ));
    QVERIFY(dmm.preset);
    QCOMPARE(dmm.setupScpi, setupScpi);
    QCOMPARE(dmm.setPointsScpi, QString("TRIG:COUN %1"));
    QCOMPARE(dmm.setTriggerDelayScpi, QString("TRIG:DEL  %1"));
    QCOMPARE(dmm.startScpi, QString("INIT"));
    QCOMPARE(dmm.sleepAfterStart_s, 100e-3);
    QCOMPARE(dmm.dataFormat, DataFormat::Ascii);
    QCOMPARE(dmm.queryDataScpi, QString("FETC?"));
    QCOMPARE(dmm.measurementType, MeasurementType::voltage);
}
void DmmDriverTest::saveTest() {
    QString filename = _temp.filePath("dmm_driver_test_save.json");
    QStringList setupScpi;
    setupScpi << "SENS:VOLT:DC:RANG 100 mV"
              << "SENS:VOLT:DC:NPLC 0.001"
              << "TRIG:SOUR EXT"
              << "TRIG:SLOP POS"
              << "SAMP:SOUR IMM"
              << "SAMP:COUN 1"
              << "FORM:DATA ASC"
              << "FORM:BORD NORM";

    DmmDriver dmm_save;
    dmm_save.driverVersion     = "2.0.0";
    dmm_save.preset            = true;
    dmm_save.setupScpi         = setupScpi;
    dmm_save.setPointsScpi     = "TRIG:COUN %1";
    dmm_save.setTriggerDelayScpi = "TRIG:DEL %1";
    dmm_save.startScpi         = "INIT";
    dmm_save.sleepAfterStart_s = 100e-3;
    dmm_save.dataFormat        = DataFormat::Ascii;
    dmm_save.queryDataScpi     = "FETC?";
    dmm_save.measurementType   = MeasurementType::voltage;
    QVERIFY(dmm_save.isValid());
    QVERIFY(dmm_save.save(filename));

    DmmDriver dmm_open(filename);
    QVERIFY(dmm_save == dmm_open);
}

void DmmDriverTest::isValidTest_data() {
    QTest::addColumn<QString>("filename");
    QTest::addColumn<bool>("isValid");
    QTest::addColumn<QString>("message");

    QTest::newRow("valid driver") << QString("valid-driver.json") << true << QString("");

    // driver version
    QTest::newRow("driver version empty")   << QString("driver-version-empty.json")   << false << QString("driver version is invalid");
    QTest::newRow("driver version invalid") << QString("driver-version-invalid.json") << false << QString("driver version is invalid");
    QTest::newRow("driver version missing") << QString("driver-version-missing.json") << false << QString("driver version is missing");

    // preset
    QTest::newRow("preset invalid") << QString("preset-invalid.json") << false << QString("invalid preset value");
    QTest::newRow("preset missing") << QString("preset-missing.json") << false << QString("preset is missing");

    // setup
//    QTest::newRow("setup scpi empty")   << QString("setup-scpi-empty.json") << false;
    QTest::newRow("setup scpi invalid") << QString("setup-scpi-invalid.json") << false << QString("invalid setup scpi array"); // TODO: Check type?
    QTest::newRow("setup scpi missing") << QString("setup-scpi-missing.json") << false << QString("setup scpi is missing");

    // set points
    QTest::newRow("set points scpi empty")   << QString("set-points-scpi-empty.json")   << false << QString("points scpi is invalid");
    QTest::newRow("set points scpi invalid") << QString("set-points-scpi-invalid.json") << false << QString("points scpi is invalid");
    QTest::newRow("set points scpi missing") << QString("set-points-scpi-missing.json") << false << QString("set points scpi is missing");

    // set trigger delay
    QTest::newRow("set trigger delay scpi empty")   << QString("set-trigger-delay-scpi-empty.json")   << false << QString("trigger delay scpi is invalid");
    QTest::newRow("set trigger delay scpi invalid") << QString("set-trigger-delay-scpi-invalid.json") << false << QString("trigger delay scpi is invalid");
    QTest::newRow("set trigger delay scpi missing") << QString("set-trigger-delay-scpi-missing.json") << false << QString("set trigger delay scpi is missing");

    // start
    QTest::newRow("start scpi empty")   << QString("start-scpi-empty.json")   << false << QString("start scpi is invalid");
    QTest::newRow("start scpi missing") << QString("start-scpi-missing.json") << false << QString("start scpi is missing");

    // sleep after start
    QTest::newRow("sleep after start scpi invalid") << QString("sleep-after-start-invalid.json") << false << QString("sleep after start scpi is invalid");
    QTest::newRow("sleep after start scpi missing") << QString("sleep-after-start-missing.json") << false << QString("sleep after start scpi is missing");

    // data format
    QTest::newRow("data format empty")       << QString("data-format-empty.json")       << false << QString("invalid data format");
    QTest::newRow("data format invalid")     << QString("data-format-invalid.json")     << false << QString("invalid data format");
    QTest::newRow("data format missing")     << QString("data-format-missing.json")     << false << QString("data format is missing");
    QTest::newRow("data format unsupported") << QString("data-format-unsupported.json") << false << QString("data format is not currently supported");

    // query data
    QTest::newRow("query data scpi empty")   << QString("query-data-scpi-empty.json")   << false << QString("query data scpi is invalid");
    QTest::newRow("query data scpi missing") << QString("query-data-scpi-missing.json") << false << QString("query data scpi is missing");

    // measurement type
    QTest::newRow("measurement type empty")       << QString("measurement-type-empty.json")       << false << QString("invalid measurement type");
    QTest::newRow("measurement type invalid")     << QString("measurement-type-invalid.json")     << false << QString("invalid measurement type");
    QTest::newRow("measurement type missing")     << QString("measurement-type-missing.json")     << false << QString("measurement type is missing");
    QTest::newRow("measurement type unsupported") << QString("measurement-type-unsupported.json") << false << QString("measurement type is not currently supported");
}
void DmmDriverTest::isValidTest() {
    QFETCH(QString, filename);
    QFETCH(bool,    isValid );
    QFETCH(QString, message);

    DmmDriver dmm(_dmmdrivertestFixtures.filePath(filename));

    QString msg;
    QCOMPARE(dmm.isValid(msg), isValid);
    QCOMPARE(msg, message);
}
