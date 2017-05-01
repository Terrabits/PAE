#ifndef MEASUREPAETEST_H
#define MEASUREPAETEST_H


// Project
#include "dmmcontroller.h"
#include "stagesettings.h"

// RsaToolbox
#include <VnaTestClass.h>

// Qt
#include <QDir>
#include <QObject>


class MeasurePaeTest : public RsaToolbox::VnaTestClass
{
    Q_OBJECT
public:
    explicit MeasurePaeTest(RsaToolbox::ConnectionType type, const QString &address, QObject *parent = 0);
    ~MeasurePaeTest();

private slots:
    void test();

private:
    QDir _src;
    QDir _fixtures;

    QVector<StageSettings> stages();
    DmmController controller();
};

#endif // MEASUREPAETEST_H
