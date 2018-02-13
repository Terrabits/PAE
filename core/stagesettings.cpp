#include "stagesettings.h"


// Project
#include "dmmdriver.h"

// RsaToolbox
#include <General.h>
using namespace RsaToolbox;

// Qt
#include <QFileInfo>
#include <QRegExp>


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

bool StageSettings::hasAcceptableInput() const {
    QString msg;
    return hasAcceptableInput(msg);
}
bool StageSettings::hasAcceptableInput(QString &message) const {
    message.clear();
    if (!hasAcceptableName(message))
        return false;
    if (!hasAcceptablePowerSupply(message))
        return false;
    if (!hasAcceptableShuntResistor(message))
        return false;
    if (!hasAcceptableConnectionType(message))
        return false;
    if (!hasAcceptableAddress(message))
        return false;
    if (!hasAcceptableDriverPath(message))
        return false;

    return true;
}

bool StageSettings::hasAcceptableName() const {
    QString msg;
    return hasAcceptableName(msg);
}
bool StageSettings::hasAcceptableName(QString &message) const {
    message.clear();
    if (name.isEmpty()) {
        message = "*stage name cannot be blank";
        return false;
    }
    QRegExp regex("^[a-z_][0-9a-z_]*$", Qt::CaseInsensitive);
    if (!regex.exactMatch(name)) {
        message = "*invalid stage name";
        return false;
    }

    return true;
}

bool StageSettings::hasAcceptablePowerSupply() const {
    QString msg;
    return hasAcceptablePowerSupply(msg);
}
bool StageSettings::hasAcceptablePowerSupply(QString &message) const {
    message.clear();
    if (powerSupply_V <= 0) {
        message = "*power supply must be >0";
        return false;
    }
    return true;
}

bool StageSettings::hasAcceptableShuntResistor() const {
    QString msg;
    return hasAcceptableShuntResistor(msg);
}
bool StageSettings::hasAcceptableShuntResistor(QString &message) const {
    message.clear();
    if (shuntResistor_ohms <= 0) {
        message = "*shunt resistor must be >0";
        return false;
    }
    return true;
}

bool StageSettings::hasAcceptableConnectionType() const {
    QString msg;
    return hasAcceptableConnectionType(msg);
}
bool StageSettings::hasAcceptableConnectionType(QString &message) const {
    message.clear();
    if (connectionType == ConnectionType::NoConnection) {
        message = "*specify valid connection type";
        return false;
    }
    return true;
}

bool StageSettings::hasAcceptableAddress() const {
    QString msg;
    return hasAcceptableAddress(msg);
}
bool StageSettings::hasAcceptableAddress(QString &message) const {
    message.clear();
    if (address.isEmpty()) {
        message = "*address must not be blank";
        return false;
    }
    return true;
}

bool StageSettings::hasAcceptableDriverPath() const {
    QString msg;
    return hasAcceptableDriverPath(msg);
}
bool StageSettings::hasAcceptableDriverPath(QString &message) const {
    message.clear();
    if (driverPath.isEmpty()) {
        message = "*driver path must not be empty";
        return false;
    }
    if (!QFileInfo(driverPath).exists()) {
        message = "*\'%1\' does not exist";
        message = message.arg(driverFilename());
        return false;
    }
    DmmDriver driver(driverPath);
    if (!driver.isOpen()) {
        message = "*could not read \'%1\': is driver valid?";
        message = message.arg(driverFilename());
        return false;
    }
    return true;
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

QDataStream &operator<<(QDataStream &stream, const StageSettings &settings) {
    stream << settings.name;
    stream << settings.powerSupply_V;
    stream << settings.shuntResistor_ohms;
    stream << qint32(settings.connectionType);
    stream << settings.address;
    stream << settings.driverPath;
    return stream;
}
QDataStream &operator>>(QDataStream &stream, StageSettings &settings) {
    qint32 _qint32;

    stream >> settings.name;
    stream >> settings.powerSupply_V;
    stream >> settings.shuntResistor_ohms;
    stream >> _qint32;
    settings.connectionType = ConnectionType(_qint32);
    stream >> settings.address;
    stream >> settings.driverPath;
    return stream;
}
