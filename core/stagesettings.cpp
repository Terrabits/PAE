#include "stagesettings.h"


// RsaToolbox
#include <General.h>
using namespace RsaToolbox;

// Qt
#include <QFileInfo>


StageSettings::StageSettings()
{
    init();
}
StageSettings::StageSettings(const QString &name) {
    init();
    this->name = name;
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

void StageSettings::init() {
    name.clear();
    powerSupply_V = 5.0;
    shuntResistor_ohms = 1.0E-3;
    connectionType = ConnectionType::VisaTcpConnection;
    address.clear();
    driverPath.clear();
}
