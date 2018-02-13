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
#include <QDebug>
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

    _logFilenames << "test log.txt";
}

MeasurePaeTest::~MeasurePaeTest()
{

}

void MeasurePaeTest::test() {
    QString setFilename;
    if (_vna->properties().isZnbFamily()) {
        setFilename = "znb-4port.znx";
    }
    else if (_vna->properties().isZvaFamily()) {
        setFilename = "zva-4port.zvx";
    }
    _vna->fileSystem().uploadFile(_fixtures.filePath(setFilename), setFilename, VnaFileSystem::Directory::RECALL_SETS_DIRECTORY);
    _vna->openSet(setFilename);
    _vna->pause();
    QVERIFY(!_vna->isError());

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
    stage.name           = "stage_1_A";
    stage.powerSupply_V  = 5.0;
    stage.shuntResistor_ohms = 1.0e-3;
    stage.connectionType = ConnectionType::VisaTcpConnection;
    stage.address        = "127.0.0.1";
    stage.driverPath     = driverPath;

    QVector<StageSettings> stages;
    stages << stage;
    stage.name = "stage_2_A";
    stages << stage;
    stage.name = "stage_3_A";
    stages << stage;
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

