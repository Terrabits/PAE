

// Project
#include "dmmcontroller.h"
#include "measurepae.h"
#include "settings.h"
#include "stagesettings.h"

// RsaToolbox
#include <Vna.h>
using namespace RsaToolbox;

// Qt
#include <QCoreApplication>
#include <QDebug>


int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    qDebug() << qFuzzyCompare(QString("0.10000000000000001").toDouble(), double(100e-3));
    return 0;

    Log log(dataDir.filePath("log.txt"), "Test", "0.0");
    log.printHeader();

    Vna vna(ConnectionType::VisaGpibConnection, "20");
    if (!vna.isConnected()) {
        qDebug() << "vna not found";
        return 1;
    }
    vna.useLog(&log);
    vna.printInfo();
    vna.isError();
    vna.clearStatus();

    vna.closeSets();
    vna.openSet("20.zvx");
    vna.pause();

    StageSettings stage1;
    stage1.name           = "Stage1";
    stage1.connectionType = ConnectionType::VisaGpibConnection;
    stage1.address        = "16";
    stage1.driverPath     = dataDir.filePath("..\\test\\fixtures\\Agilent 34410A.json");
    stage1.powerSupply_V  = 10.0;
    stage1.shuntResistor_ohms = 50.0;

    QString msg;
    if (!stage1.hasAcceptableInput(msg)) {
        qDebug() << msg;
        return 1;
    }

    QVector<StageSettings> stages;
    stages << stage1;

    MeasurePAE pae;
    pae.setVna(&vna);
    pae.setCalculation(MeasurePAE::Calculation::drainEfficiency);
    pae.setInputTrace("Trc1");
    pae.setGainTrace("Trc2");
    pae.setStages(stages);

    if (!pae.hasAcceptableInput(msg)) {
        qDebug() << msg;
        return 1;
    }

    pae.run();
    return 0;
}
