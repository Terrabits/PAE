#include "stagesettings.h"


// RsaToolbox
#include <General.h>
using namespace RsaToolbox;

// Qt
#include <QFileInfo>


StageSettings::StageSettings()
{
//    name.clear();
    powerSupply_V = 5.0;
    shuntResistor_ohms = 1.0e-3;
    connectionType = ConnectionType::VisaTcpConnection;
//    address.clear();
//    driverPath.clear();
}

StageSettings::~StageSettings()
{

}

QString StageSettings::connectionTypeString() const {
    if (connectionType == ConnectionType::NoConnection)
        return QString();

    return toString(connectionType);
}
QString StageSettings::driverFilename() const {
    if (driverPath.isEmpty())
        return QString();

    return QFileInfo(driverPath).fileName();
}
