

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
//    vna.preset();
    vna.clearStatus();
    vna.pause();

    vna.channel().linearSweep().setPoints(201);

    TraceSettings settings;
    settings.name = "PAE_TRACE";
    settings.channel    = 1;
    settings.diagram    = 1;
    settings.outputPort = 2;
    settings.inputPort  = 1;
    settings.data = QRowVector(201, 0.1);

    ProcessTrace(settings, &vna);

    return 0;
}
