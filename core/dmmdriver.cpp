#include "dmmdriver.h"

// Qt
#include <QByteArray>
#include <QDebug>
#include <QFile>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QVariantList>

#include <cassert>

DmmDriver::DmmDriver() {
    init();
}
DmmDriver::DmmDriver(const DmmDriver &other) {
    copy(other);
}
DmmDriver::DmmDriver(const QString &filename) {
    open(filename);
}

bool DmmDriver::isOpen() const {
    return _isOpen;
}
bool DmmDriver::isValid() const {
    QString msg;
    return isValid(msg);
}
bool DmmDriver::isValid(QString &errorMessage) const {
    errorMessage.clear();

    // Open?
//    if (!isOpen()) {
//        errorMessage = "file not open";
//        return false;
//    }
    if (isOpenError()) {
        errorMessage = _openError;
        return false;
    }


    // Valid values?
    if (!validDriverVersion()) {
        errorMessage = "driver version is invalid";
        return false;
    }
    if (majorVersion() != 2) {
        errorMessage = "driver major version number must be 2";
        return false;
    }
    if (!validSetPointsScpi()) {
        errorMessage = "points scpi is invalid";
        return false;
    }
    if (!validSetTriggerDelayScpi()) {
        errorMessage = "trigger delay scpi is invalid";
        return false;
    }
    if (!validStartScpi()) {
        errorMessage = "start scpi is invalid";
        return false;
    }
    if (!validSleepAfterStart()) {
        errorMessage = "sleep after start scpi is invalid";
        return false;
    }
    if (!validQueryDataScpi()) {
        errorMessage = "query data scpi is invalid";
        return false;
    }

    // Supported features
    if (dataFormat != DataFormat::Ascii) {
        errorMessage = "data format is not currently supported";
        return false;
    }
    if (measurementType != MeasurementType::voltage) {
        errorMessage = "measurement type is not currently supported";
        return false;
    }

    // validated
    return true;
}
QString DmmDriver::filename() const {
    return _filename;
}

uint DmmDriver::majorVersion() const {
    return driverVersion.split(".")[0].toUInt();
}
uint DmmDriver::minorVersion() const {
    return driverVersion.split(".")[1].toUInt();
}
uint DmmDriver::bugVersion() const {
    return driverVersion.split(".")[2].toUInt();
}

bool DmmDriver::save(const QString &filename) {
    QFile file(filename);
    if (!file.open(QFile::WriteOnly)) {
        return false;
    }

    QJsonObject json;
    json["driver version"]    = driverVersion;
    json["preset"]           = preset;
    json["setup scpi"]       = QJsonArray::fromStringList(setupScpi);
    json["set points scpi"]  = setPointsScpi;
    json["set trigger delay scpi"] = setTriggerDelayScpi;
    json["start scpi"]       = startScpi;
    json["sleep after start scpi"] = sleepAfterStart_s;
    json["data format"]      = toString(dataFormat);
    json["query data scpi"]  = queryDataScpi;
    json["measurement type"] = toString(measurementType);

    if (!file.write(QJsonDocument(json).toJson())) {
        file.close();
        return false;
    }
    file.close();
    return true;
}

QString DmmDriver::toString(const MeasurementType &type) {
    if (type == MeasurementType::current)
        return "current";
    else
        return "voltage";
}
MeasurementType DmmDriver::toMeasurementType(QString string, bool *ok) {
    string = string.trimmed().toLower();
    if (ok) {
        *ok = false;
    }

    if (string== "current") {
        if (ok) {
            *ok = true;
        }
        return MeasurementType::current;
    }
    if (string == "voltage") {
        if (ok) {
            *ok = true;
        }
        return MeasurementType::voltage;
    }
    // default
    return MeasurementType::voltage;
}

QString DmmDriver::toString(const DataFormat &format) {
    switch (format) {
    case DataFormat::Ascii:
        return "ascii";
    case DataFormat::Binary32Bit:
        return "binary 32 bit";
    case DataFormat::Binary64Bit:
        return "binary 64 bit";
    default:
        return "ascii";
    }
}
DataFormat DmmDriver::toDataFormat(QString string, bool *ok) {
    string = string.trimmed().toLower();
    if (ok) {
        *ok = false;
    }
    if (string == "ascii") {
        if (ok) {
            *ok = true;
        }
        return DataFormat::Ascii;
    }
    if (string == "binary 32 bit") {
        if (ok) {
            *ok = true;
        }
        return DataFormat::Binary32Bit;
    }
    if (string == "binary 64 bit") {
        if (ok) {
            *ok = true;
        }
        return DataFormat::Binary64Bit;
    }
    // default
    return DataFormat::Ascii;
}

void DmmDriver::operator=(const DmmDriver &other) {
    copy(other);
}

bool DmmDriver::isOpenError() const {
    return !_openError.trimmed().isEmpty();
}

void DmmDriver::init() {
    _isOpen = false;
    _filename.clear();
    _openError.clear();
    clearKeys();
}

void DmmDriver::clearKeys() {
    driverVersion.clear();
    preset            = false;
    setupScpi.clear();
    setPointsScpi.clear();
    startScpi.clear();
    sleepAfterStart_s = 0.0;
    dataFormat        = DataFormat::Ascii;
    queryDataScpi.clear();
    measurementType   = MeasurementType::voltage;
}

bool DmmDriver::open(const QString &filename) {
    init();

    _filename = filename;
    QFile file(filename);
    if (!file.open(QFile::ReadOnly)) {
        _openError = "file not found";
        return false;
    }
    QByteArray text = file.readAll();
    file.close();

    if (text.isEmpty()) {
        _openError = "file is empty";
        return false;
    }

    QJsonParseError e;
    QJsonDocument document = QJsonDocument::fromJson(text, &e);
    if (e.error != QJsonParseError::ParseError::NoError) {
        _openError = e.errorString();
        return false;
    }
    if (!document.isObject()) {
        _openError = "could not parse file";
        return false;
    }

    // Check for keys
    QJsonObject json  = document.object();
    QStringList keys;
    keys << "driver version"
         << "preset"
         << "setup scpi"
         << "set points scpi"
         << "set trigger delay scpi"
         << "start scpi"
         << "sleep after start scpi"
         << "data format"
         << "query data scpi"
         << "measurement type";
    foreach (const QString &key, keys) {
        if (!json.contains(key)) {
            _openError = "%1 is missing";
            _openError = _openError.arg(key);
            return false;
        }
    }

    // Check expected data types
    if (!json.value("driver version").isString()) {
        _openError = "invalid driver version";
        return false;
    }
    if (!json.value("preset").isBool()) {
        _openError = "invalid preset value";
        return false;
    }
    if (!json.value("setup scpi").isArray()) {
        _openError = "invalid setup scpi array";
        return false;
    }
    if (!json.value("set points scpi").isString()) {
        _openError = "invalid set points scpi";
        return false;
    }
    if (!json.value("set trigger delay scpi").isString()) {
        _openError = "invalid set trigger delay scpi";
        return false;
    }
    if (!json.value("start scpi").isString()) {
        _openError = "invalid start scpi";
        return false;
    }
    if (!json.value("sleep after start scpi").isDouble()) {
        _openError = "invalid sleep after start scpi";
        return false;
    }
    if (!json.value("data format").isString()) {
        _openError = "invalid data format";
        return false;
    }
    if (!json.value("query data scpi").isString()) {
        _openError = "invalid query data scpi";
        return false;
    }
    if (!json.value("measurement type").isString()) {
        _openError = "invalid measurement type";
        return false;
    }

    // Get values
    bool ok;
    driverVersion     = json.value("driver version").toString();
    preset            = json.value("preset").toBool();
    setupScpi         = json.value("setup scpi").toVariant().toStringList();
    setPointsScpi     = json["set points scpi"].toString();
    setTriggerDelayScpi = json["set trigger delay scpi"].toString();
    startScpi         = json["start scpi"].toString();
    sleepAfterStart_s = json["sleep after start scpi"].toDouble();
    dataFormat        = toDataFormat(json["data format"].toString(), &ok);
    if (!ok) {
        _openError = "invalid data format";
        clearKeys();
        return false;
    }
    queryDataScpi     = json["query data scpi"].toString();
    measurementType   = toMeasurementType(json["measurement type"].toString(), &ok);
    if (!ok) {
        _openError = "invalid measurement type";
        clearKeys();
        return false;
    }

    _isOpen           = true;
    return true;
}

bool DmmDriver::validDriverVersion() const {
    if (driverVersion.isEmpty()) {
        return false;
    }
    const QStringList parts = driverVersion.split(".");
    if (parts.size() != 3) {
        return false;
    }
    bool ok = false;
    parts[0].toUInt(&ok);
    if (!ok) {
        return false;
    }
    parts[1].toUInt(&ok);
    if (!ok) {
        return false;
    }
    parts[2].toUInt(&ok);
    if (!ok) {
        return false;
    }

    return true;
}
bool DmmDriver::validSetPointsScpi() const {
    if (setPointsScpi.trimmed().isEmpty()) {
        return false;
    }
    if (!setPointsScpi.contains("%1")) {
        return false;
    }
    return true;
}
bool DmmDriver::validSetTriggerDelayScpi() const {
    if (setTriggerDelayScpi.trimmed().isEmpty()) {
        return false;
    }
    if (!setTriggerDelayScpi.contains("%1")) {
        return false;
    }
    return true;
}
bool DmmDriver::validStartScpi() const {
    if (startScpi.trimmed().isEmpty()) {
        return false;
    }
    return true;
}
bool DmmDriver::validSleepAfterStart() const {
    if (sleepAfterStart_s < 0) {
        return false;
    }
    return true;
}
bool DmmDriver::validQueryDataScpi() const {
    if (queryDataScpi.trimmed().isEmpty()) {
        return false;
    }
//    if (!queryDataScpi.contains("?")) {
//        return false;
//    }
    return true;
}

void DmmDriver::copy(const DmmDriver &other) {
    _filename         = other._filename;
    _isOpen           = other._isOpen;
    _openError        = other._openError;

    driverVersion     = other.driverVersion;
    preset            = other.preset;
    setupScpi         = other.setupScpi;
    setPointsScpi     = other.setPointsScpi;
    setTriggerDelayScpi = other.setTriggerDelayScpi;
    startScpi         = other.startScpi;
    sleepAfterStart_s = other.sleepAfterStart_s;
    dataFormat        = other.dataFormat;
    queryDataScpi     = other.queryDataScpi;
    measurementType   = other.measurementType;
}

bool operator==(const DmmDriver &left, const DmmDriver &right) {

    if (left.driverVersion != right.driverVersion) {
        return false;
    }
    if (left.preset != right.preset) {
        return false;
    }

    // Compare setup scpi.
    // Order is important?
    QStringList leftSetupScpi = left.setupScpi;
    for (int i = 0; i < leftSetupScpi.size(); i++) {
        leftSetupScpi[i] = leftSetupScpi[i].toLower();
    }
//    std::sort(leftSetupScpi.begin(), leftSetupScpi.end());

    QStringList rightSetupScpi = right.setupScpi;
    for (int i = 0; i < rightSetupScpi.size(); i++) {
        rightSetupScpi[i] = rightSetupScpi[i].toLower();
    }
//    std::sort(rightSetupScpi.begin(), rightSetupScpi.end());

    if (leftSetupScpi != rightSetupScpi) {
        return false;
    }

    if (left.setPointsScpi.toLower() != right.setPointsScpi.toLower()) {
        return false;
    }
    if (left.startScpi.toLower() != right.startScpi.toLower()) {
        return false;
    }
    if (!qFuzzyCompare(left.sleepAfterStart_s, right.sleepAfterStart_s)) {
        return false;
    }
    if (left.dataFormat != right.dataFormat) {
        return false;
    }
    if (left.queryDataScpi.toLower() != right.queryDataScpi.toLower()) {
        return false;
    }
    if (left.measurementType != right.measurementType) {
        return false;
    }

    // Same as it ever was
    return true;
}
bool operator!=(const DmmDriver &left, const DmmDriver &right) {
    return !(left == right);
}
