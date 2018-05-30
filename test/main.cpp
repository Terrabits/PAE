

// Project
#include "settings.h"

// Tests
#include "dmmcontrollertest.h"
#include "dmmdrivertest.h"
#include "dmmtest.h"
#include "measurepaetest.h"

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
    testRunner.addTest(new DmmTest);
    testRunner.addTest(new DmmControllerTest);
//    testRunner.addTest(new MeasurePaeTest(CONNECTION_TYPE, INSTRUMENT_ADDRESS));

    qDebug() << "Global result: " << (testRunner.runTests() ? "PASS" : "FAIL");
    return 0;
}
