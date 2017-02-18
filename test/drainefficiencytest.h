#ifndef DRAINEFFICIENCYTEST_H
#define DRAINEFFICIENCYTEST_H

// RsaToolbox
#include <TestClass.h>

// Qt
#include <QObject>

class DrainEfficiencyTest : public RsaToolbox::TestClass
{
    Q_OBJECT

public:
    explicit DrainEfficiencyTest(QObject *parent = 0);

private slots:
    void calculate();
};

#endif // DRAINEFFICIENCYTEST_H
