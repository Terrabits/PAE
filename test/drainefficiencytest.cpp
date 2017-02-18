#include "drainefficiencytest.h"

// Project
#include "drainefficiency.h"
#include "keysightdmm.h"

// RsaToolbox
#include <Definitions.h>
#include <Vna.h>
using namespace RsaToolbox;

DrainEfficiencyTest::DrainEfficiencyTest(QObject *parent) :
    TestClass(parent)
{

}

void DrainEfficiencyTest::calculate() {
    Vna vna(ConnectionType::VisaGpibConnection, "10");
    KeysightDmm dmm(ConnectionType::VisaGpibConnection, "11");

    VnaChannel ch1 = vna.channel(1);
    uint points = ch1.linearSweep().points();

    dmm.setRange("100 mV");
    dmm.setNplc(0.001);
    dmm.setTriggerSource("EXT");
    dmm.setTriggerDelay(10E-6);
    dmm.setTriggerCount(points);
    dmm.setTriggerSlope("POS");
    dmm.setSampleSource("IMM");
    dmm.setSampleCount(1);
    dmm.setDataFormat("ASC");
    QVERIFY(!dmm.isError());

    dmm.startMeasurement();
    QVERIFY(!dmm.isError());

    ch1.startSweep();

    QRowVector supply_V = dmm.results(points);
    QCOMPARE(supply_V.size(), int(points));

    QRowVector pout_dBm;
    vna.trace("Trc2").y(pout_dBm);

    DrainEfficiency de;
    de.setR(5.0E-3);
    de.setVdd(50.0);

    QRowVector result = de.calculatePercent(supply_V, pout_dBm);
    ComplexRowVector complexResult;
    complexResult.resize(points, ComplexDouble(1,0));
    for (uint i = 0; i < points; i++) {
        complexResult[i] = ComplexDouble(result[i], 0);
    }

    VnaTrace deTrc = vna.trace("drain_e");
    deTrc.write(complexResult);
}
