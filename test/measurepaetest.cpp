#include "measurepaetest.h"

// Project
#include "dmm.h"
#include "measurepae.h"
#include "stageresult.h"

// RsaToolbox
#include <General.h>
#include <MockBus.h>
using namespace RsaToolbox;

// Qt
#include <QScopedPointer>
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

    measure.run();

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
}

QVector<StageSettings> MeasurePaeTest::stages() {
    const QString driverPath = _fixtures.filePath("../example_driver.json");
    StageSettings stage;
    stage.powerSupply_V = 5.0;
    stage.shuntResistor_ohms = 1.0e-3;
    stage.connectionType = ConnectionType::VisaTcpConnection;
    stage.address        = "127.0.0.1";
    stage.driverPath = driverPath;

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
    for (int i = 0; i < points; i++) {
        int j = ports.size() * i + sourceIndex;
        read[j] = 1.0e-6;

    }
    const QString readString = toString(read, ",");
    QVariantList readList;
    readList << QVariant(readString);

    DmmController controller;
    controller.setStages(stages());
    controller.setSweepPoints(points);
    controller.setPorts(ports, sourcePort);

    for (int i = 0; i < controller._dmms.size(); i++) {
        Dmm &dmm = *controller._dmms[i];
        QScopedPointer<MockBus> bus(new MockBus);
        bus->setReads(readList);
        dmm.resetBus(bus.take());
    }

    return controller;
}
