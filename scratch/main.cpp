

// Project
#include "processtrace.h"
#include "tracesettings.h"

// RsaToolbox
#include <Vna.h>
using namespace RsaToolbox;

// Qt
#include <QCoreApplication>
#include <QDebug>


int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    Vna vna(ConnectionType::VisaTcpConnection, "127.0.0.1");
    vna.isError();
    vna.preset();
    vna.clearStatus();
    vna.pause();

    VnaChannel ch1 = vna.channel(1);
    ch1.linearSweep().setPoints(1);

    vna.createTrace("_stage_1_A", 1);
    VnaTrace dataTrc = vna.trace("_stage_1_A");
    dataTrc.setParameter("PAE21");
    dataTrc.toMemory("stage_1_A");
    VnaTrace memTrc = vna.trace("stage_1_A");

    vna.createDiagram(2);
    dataTrc.setDiagram(2);
    memTrc .setDiagram(2);


    ComplexRowVector ycomp(1, ComplexDouble(1,0));
    memTrc.write(ycomp);

    QRowVector y;
    memTrc.y(y);
    qDebug() << y[0];
    qDebug() << memTrc.y()[0].real() << memTrc.y()[0].imag();

    return 0;
}
