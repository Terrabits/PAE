#include "measurepaetest.h"

// Project
#include "dmm.h"
#include "measurepae.h"
#include "stageresult.h"

// RsaToolbox
#include <General.h>
#include <MockBus.h>
#include <Test.h>
using namespace RsaToolbox;

// Qt
#include <QScopedPointer>
#include <QSignalSpy>
#include <QVariant>
#include <QVector>


MeasurePaeTest::MeasurePaeTest(ConnectionType type, const QString &address, QObject *parent) :
    VnaTestClass(type, address, parent)
{
    _src.setPath(SOURCE_DIR);
    _fixtures.setPath(_src.filePath("fixtures/measurepaetest"));
    _src.mkpath("logs/measurepaetest");
    _logDir.setPath(_src.filePath("logs/measurepaetest"));

    _logFilenames << "test.txt";
}

MeasurePaeTest::~MeasurePaeTest()
{

}

void MeasurePaeTest::test() {
    if (_vna->properties().isZnbFamily()) {
        _vna->openSet(_fixtures.filePath("znb8-4port"));
    }
    else if (_vna->properties().isZvaFamily()) {
        _vna->openSet(_fixtures.filePath("zva"));
    }
    _vna->pause();

    MeasurePAE measure;
    measure.setVna(_vna.data());
    measure.setCalculation(MeasurePAE::Calculation::powerAddedEfficiency);
    measure.setInputTrace("a1");
    measure.setOutputTrace("b2");
    measure.setStages(stages());
    measure._controller = controller();

    QString msg;
    measure.hasAcceptableInput(msg);
    QCOMPARE(msg, QString());

    QSignalSpy error(&measure, SIGNAL(error(QString)));

    measure.run();
    if (!error.isEmpty()) {
        QList<QVariant> errors = error.takeFirst();
        qDebug() << "Error: " << errors.first().toString();
    }
    QVERIFY(error.isEmpty());
    pause("Check traces");

    const int points = _vna->channel(1).linearSweep().points();
    QVector<StageResult> results = measure.stageResults();
    QVERIFY(!results.isEmpty());
    for (int i = 0; i < results.size(); i++) {
        QCOMPARE(results[i].current_A().size(), points);
        QCOMPARE(results[i].voltage_V().size(), points);
        QCOMPARE(results[i].power_W().size(), points);
    }
    QCOMPARE(measure.dcPower_W().size(), points);
    QCOMPARE(measure.efficiency_pct().size(), points);

    QVERIFY(!_vna->isError());
}

QVector<StageSettings> MeasurePaeTest::stages() {
    const QString driverPath = _fixtures.filePath("../example_driver.json");
    StageSettings stage;
    stage.name           = "stage_1";
    stage.powerSupply_V  = 5.0;
    stage.shuntResistor_ohms = 1.0e-3;
    stage.connectionType = ConnectionType::VisaTcpConnection;
    stage.address        = "127.0.0.1";
    stage.driverPath     = driverPath;

    const uint numStages = 3;
    QVector<StageSettings> stages(numStages, stage);
    return stages;
}

DmmController MeasurePaeTest::controller() {
    const uint points      = _vna->channel(1).linearSweep().points();
    const uint sourcePort  = 2;
    QVector<uint> ports;
    ports << 1 << 2 << 3 << 4;
    const uint sourceIndex = ports.indexOf(sourcePort);
    QRowVector read(points * ports.size(), 0);
    for (uint i = 0; i < points; i++) {
        int j = ports.size() * i + sourceIndex;
        read[j] = 1.0e-6;
    }
    const QString readString = toString(read, ",");
    QVariantList readList;
    readList << QVariant("fake id string");
    readList << QVariant(readString);

    DmmController controller;
    controller.setStages(stages());
    controller.setSweepPoints(points);
    controller.setPorts(ports, sourcePort);

    for (int i = 0; i < controller._dmms.size(); i++) {
        Dmm &dmm = *controller._dmms[i];
        QScopedPointer<MockBus> bus(new MockBus(ConnectionType::VisaTcpConnection, "192.168.1.1"));
        bus->setReads(readList);
        dmm.resetBus(bus.take());
    }

    return controller;
}

//QStringList MeasurePaeTest::vnaReads() {
//    QMap<QString, QString> map;
//    map["*IDN?"] = "Rohde-Schwarz,ZNB20-4Port,1311601064100104,2.70";
//    map["*OPT?"] = "ZNB-K2,ZNB-K4,ZNB-K14,ZNB-K17,ZNB-K19,ZNB-K20,ZNB8-B24,ZNB8-B31,ZNB8-B32,ZNB8-B33,ZNB8-B34,ZNB-B81,ZNB-B2,ZN-B15";
//    map["*OPC?"] = "1";
//    map[":MMEM:CDIR?"] = "\'C:\Users\Public\Documents\Rohde-Schwarz\VNA\'";
//    map[":SENS1:SWE:POIN?"] = "201";
//    map[":CONF:TRAC:CHAN:NAME:ID? \'a1\'"] = "1";
//    map[":CONF:TRAC:CHAN:NAME:ID? \'b2\'"] = "1";
//    map[":SENS1:SWE:TYPE?"] = "LIN";
//    map[":SENS1:SWE:TIME?"] = "0.0175875";
//    map[":SYST:FREQ? MAX"] = "20000000000";
//    map[":DISP:CAT?"] = "\'1,1,2,2,3,3\'";
//    map[":CALC1:PAR:MEAS? \'b2\'"] = "\'B2D1SAM\'";
//    map[":CALC1:PAR:MEAS? \'a1\'"] = "\'A1D1SAM'";
////    map[":CONF:TRAC:CAT?"] = ["\'1,S21,2,a1,3,b2\'", "\'1,S21,2,a1,3,b2,4,_stage_1,5,stage_1\'", "\'1,S21,2,a1,3,b2,4,_stage_1,5,stage_1\'", "\'1,S21,2,a1,3,b2,4,_stage_1,5,stage_1\'", "\'1,S21,2,a1,3,b2,4,_stage_1,5,stage_1\'", "\'1,S21,2,a1,3,b2,4,_stage_1,5,stage_1\'", "\'1,S21,2,a1,3,b2,4,_stage_1,5,stage_1\'", "\'1,S21,2,a1,3,b2,4,_stage_1,5,stage_1\'"];
//    map[":SOUR1:LPOR1?"] = "1";
//    map[":CONF:TRAC:CHAN:NAME:ID? \'_stage_1\'"] = "1";
//    map[":CONF:TRAC:CHAN:NAME:ID? \'stage_1\'"] = "1";
//    map[":CALC1:FORM?"] = "MLIN";
//    map[":INIT1:CONT?"] = "1";
//    map[":FORM?"] = "REAL,64";
//    map[":FORM:BORD?"] = "SWAP";
//    map[":CALC:DATA:TRAC? \'a1\', FDAT"] = "TODO";
//    map[":CALC:DATA:TRAC? \'b2\', FDAT"] = "TODO";
//    map[":CONF:TRAC:CHAN:NAME:ID? \'_PAE\'"] = "1";
//    map[":CONF:TRAC:CHAN:NAME:ID? \'PAE\'"] = "1";
//    map[""] = "";
//    map[""] = "";
//    map[""] = "";
//    map[""] = "";


//    map[""] = "";
//}
