#include "dmmdriver.h"

// Qt
#include <QByteArray>
#include <QDebug>
#include <QFile>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QVariantList>

DmmDriver::DmmDriver()
{
    init();
}
DmmDriver::DmmDriver(const DmmDriver &other) {
    setupScpi         = other.setupScpi;
    setPointsScpi     = other.setPointsScpi;
    startScpi         = other.startScpi;
    sleepAfterStart_s = other.sleepAfterStart_s;
    queryDataScpi     = other.queryDataScpi;
    measurementType   = other.measurementType;
}

DmmDriver::DmmDriver(const QString &filename)
{
    open(filename);
}

bool DmmDriver::isOpen() const {
    return _isOpen;
}
QString DmmDriver::filename() const {
    return _filename;
}

bool DmmDriver::save(const QString &filename) {
    QFile file(filename);
    if (!file.open(QFile::WriteOnly)) {
        return false;
    }

    QJsonObject json;
    json["setup scpi"]       = QJsonArray::fromStringList(setupScpi);
    json["set points scpi"]  = setPointsScpi;
    json["start scpi"]       = startScpi;
    json["sleep after start scpi"] = sleepAfterStart_s;
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
MeasurementType DmmDriver::toMeasurementType(const QString &string) {
    if (string.toLower() == "current")
        return MeasurementType::current;
    else
        return MeasurementType::voltage;
}

void DmmDriver::operator=(const DmmDriver &other) {
    setupScpi         = other.setupScpi;
    setPointsScpi     = other.setPointsScpi;
    startScpi         = other.startScpi;
    sleepAfterStart_s = other.sleepAfterStart_s;
    queryDataScpi     = other.queryDataScpi;
    measurementType   = other.measurementType;
}

void DmmDriver::init() {
    _isOpen = false;
    _filename.clear();

    setupScpi.clear();
    setPointsScpi.clear();
    startScpi.clear();
    sleepAfterStart_s = 100E-3;
    queryDataScpi.clear();
    measurementType = MeasurementType::voltage;
}

bool DmmDriver::open(const QString &filename) {
    init();

    _filename = filename;
    QFile file(filename);
    if (!file.open(QFile::ReadOnly)) {
        _isOpen = false;
        return false;
    }
    QByteArray text = file.readAll();
    file.close();

    QJsonParseError e;
    QJsonDocument document = QJsonDocument::fromJson(text, &e);
    if (e.error != QJsonParseError::ParseError::NoError) {
        _isOpen = false;
        return false;
    }
    if (!document.isObject()) {
        _isOpen = false;
        return false;
    }

    QJsonObject json  = document.object();
    setupScpi         = json.value("setup scpi").toVariant().toStringList();
    setPointsScpi     = json["set points scpi"].toString();
    startScpi         = json["start scpi"].toString();
    sleepAfterStart_s = json["sleep after start scpi"].toDouble();
    queryDataScpi     = json["query data scpi"].toString();
    measurementType   = toMeasurementType(json["measurement type"].toString());
    return true;
}

bool operator==(const DmmDriver &left, const DmmDriver &right) {
    if (left.setupScpi != right.setupScpi)
        return false;
    if (left.setPointsScpi != right.setPointsScpi)
        return false;
    if (left.startScpi != right.startScpi)
        return false;
    if (left.sleepAfterStart_s != right.sleepAfterStart_s)
        return false;
    if (left.queryDataScpi != right.queryDataScpi)
        return false;
    if (left.measurementType != right.measurementType)
        return false;

    // Same as it ever was
    return true;
}
bool operator!=(const DmmDriver &left, const DmmDriver &right) {
    return !(left == right);
}
