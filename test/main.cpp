

// Project
#include "settings.h"

// Tests
#include "dmmdrivertest.h"
#include "drainefficiencytest.h"
#include "keysightdmmtest.h"

// RsaToolbox
#include <General.h>
#include <TestRunner.h>
using namespace RsaToolbox;

// Qt
#include <QtTest>
#include <QDebug>


int main() {
    TestRunner testRunner;
    testRunner.addTest(new DmmDriverTest);
//    testRunner.addTest(new DrainEfficiencyTest);
//    testRunner.addTest(new KeysightDmmTest(CONNECTION_TYPE, INSTRUMENT_ADDRESS,
//                                           DMM_CONNECTION_TYPE, DMM_ADDRESS));

    qDebug() << "Global result: " << (testRunner.runTests() ? "PASS" : "FAIL");
    return 0;
}
