

// Project
#include "settings.h"

// Tests
#include "dmmdrivertest.h"

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

    qDebug() << "Global result: " << (testRunner.runTests() ? "PASS" : "FAIL");
    return 0;
}
