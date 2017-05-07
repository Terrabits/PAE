#include <QCoreApplication>

// RsaToolbox
#include <Vna.h>
using namespace RsaToolbox;

// Qt
#include <QDebug>


int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    Vna vna(ConnectionType::VisaTcpConnection, "127.0.0.1");
    VnaTrace data = vna.trace("Trc1");
    VnaTrace mem  = vna.trace("Mem2[Trc1]");

    ComplexRowVector x(201, ComplexDouble(75,0));
    mem.write(x);

    qDebug() << data.parameter();
//    return a.exec();
    return 0;
}
