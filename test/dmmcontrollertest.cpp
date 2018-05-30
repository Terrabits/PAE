#include "dmmcontrollertest.h"


// Project
#include "dmm.h"
#include "dmmcontroller.h"
#include "stageresult.h"
#include "stagesettings.h"

// RsaToolbox
#include <General.h>
#include <MockBus.h>
using namespace RsaToolbox;

// Qt
#include <QDebug>
#include <QSharedPointer>
#include <QVector>


DmmControllerTest::DmmControllerTest(QObject *parent) : TestClass(parent)
{
    _src.setPath(SOURCE_DIR);
    _fixtures.setPath(_src.filePath("fixtures"));
}

DmmControllerTest::~DmmControllerTest()
{

}

void DmmControllerTest::test() {
    const QString driverPath = _fixtures.filePath("example_driver.json");
    StageSettings stage;
    stage.powerSupply_V = 5.0;
    stage.shuntResistor_ohms = 1.0e-3;
    stage.connectionType = ConnectionType::VisaGpibConnection;
    stage.address        = "16";
    stage.driverPath = driverPath;

    const uint numStages = 3;
    QVector<StageSettings> stages(numStages, stage);

    const uint points      = 101;
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
    readList << QString("1");
    readList << QString("1");
    readList << QVariant(readString);

    DmmController controller;
    controller.setStages(stages);
    controller.setSweepPoints(points);
    controller.setTriggerDelay(1e-3);
    controller.setPorts(ports, sourcePort);

    for (int i = 0; i < controller._dmms.size(); i++) {
        Dmm &dmm = *controller._dmms[i];
        QScopedPointer<MockBus> bus(new MockBus);
        bus->setReads(readList);
        dmm.resetBus(bus.take());
    }

    controller.setup();
    controller.start();
    QVector<StageResult> results = controller.readResults();
    for (int i = 0; i < results.size(); i++) {
        QCOMPARE(results[i].voltage_V().size(), int(points));
        QCOMPARE(results[i].voltage_V(), QRowVector(points, (5.0 - 1.0e-6)));
    }
}
