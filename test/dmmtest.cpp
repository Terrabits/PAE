#include "dmmtest.h"


// Project
#include <dmm.h>

// RsaToolbox
#include <General.h>
#include <MockBus.h>
using namespace RsaToolbox;

// Qt
#include <QStringList>


DmmTest::DmmTest(QObject *parent) :
    TestClass(parent)
{
    _src.setPath(SOURCE_DIR);
    _fixtures.setPath(_src.filePath("fixtures"));
}

DmmTest::~DmmTest()
{

}

void DmmTest::test() {
    // Driver
    DmmDriver driver(_fixtures.filePath("example_driver.json"));
    const uint points = 101;

    // Reads
    QRowVector data(points);
    QVariantList reads;
    reads << toString(data, ",");

    // Writes
    QVariantList setupWrites;
    for (int i = 0; i < driver.setupScpi.size(); i++) {
        setupWrites << QVariant(driver.setupScpi[i]);
    }
    setupWrites << driver.setPointsScpi.arg(points);

    QVariantList startWrites;
    startWrites << QVariant(driver.startScpi);

    QVariantList readDataWrites;
    readDataWrites << QVariant(driver.queryDataScpi);

    // Mock bus
    MockBus *bus = new MockBus;
    bus->setReads(reads);

    // Dmm
    Dmm dmm;
    dmm.resetBus(bus);
    dmm.setDriver(driver);

    // Start testing
    dmm.setup(101);
    QVERIFY(bus->isReadsLeft());
    QCOMPARE(bus->writes(), setupWrites);

    bus->clearWrites();
    dmm.start();
    QVERIFY(bus->isReadsLeft());
    QCOMPARE(bus->writes(), startWrites);

    bus->clearWrites();
    QRowVector readData = dmm.readData(101);
    QVERIFY(!bus->isReadsLeft());
    QCOMPARE(bus->writes(), readDataWrites);
    QCOMPARE(data, readData);
}
