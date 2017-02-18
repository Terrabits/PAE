#include "dmmdriver.h"

// Qt
#include <QByteArray>
#include <QDebug>
#include <QFile>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>

DmmDriver::DmmDriver()
{
    init();
}

DmmDriver::DmmDriver(const QString &filename)
{
    init();
    open(filename);
}

bool DmmDriver::save(const QString &filename) {
    QFile file(filename);
    if (!file.open(QFile::WriteOnly))
        return false;

    QJsonObject json;
    json.insert("resource string", resourceString);
    json.insert("supply voltage",  supply_V);
    if (measurementType == MeasurementType::voltage) {
        json.insert("measure",     QString("V"));
    }
    else {
        json.insert("measure",     QString("I"));
    }
    json.insert("resistor",   resistor_Ohms);
    json.insert("setup",      QJsonArray::fromStringList(setupScpi));
    json.insert("set points", pointsScpi);
    json.insert("start", startScpi);
    json.insert("sleep", sleep_s);
    json.insert("read",  readScpi);

    file.write(QJsonDocument(json).toJson());
    file.close();
    return true;
}
void DmmDriver::init() {
    supply_V        = 1;
    measurementType = MeasurementType::voltage;
    resistor_Ohms   = 1;
    sleep_s         = 100E-3;
}

bool DmmDriver::open(const QString &filename) {
    QFile file(filename);
    if (!file.open(QFile::ReadOnly))
        return false;
    QByteArray text = file.readAll();
    file.close();

    QJsonParseError e;
    QJsonDocument document = QJsonDocument::fromJson(text, &e);
    if (e.error != QJsonParseError::ParseError::NoError) {
//        qDebug() << "JSON ERROR: " << e.errorString();
        return false;
    }
    if (!document.isObject()) {
        return false;
    }

    QJsonObject json = document.object();
    if (json.contains("resource string")) {
        resourceString = json["resource string"].toString();
    }
    if (json.contains("supply voltage")) {
        supply_V = json["supply voltage"].toDouble();
    }
    if (json.contains("measure")) {
        QString measure = json["measure"].toString().trimmed().toUpper();
         if (measure == "V")
             measurementType = MeasurementType::voltage;
         else
             measurementType = MeasurementType::current;
    }
    if (json.contains("resistor")) {
        resistor_Ohms = json["resistor"].toDouble();
    }
    if (json.contains("setup")) {
        setupScpi.clear();
        QJsonArray array = json["setup"].toArray();
        for (int i = 0; i < array.size(); i++)
            setupScpi << array[i].toString();
    }
    if (json.contains("set points")) {
        pointsScpi = json["set points"].toString();
    }
    if (json.contains("start")) {
        startScpi = json["start"].toString();
    }
    if (json.contains("sleep")) {
        sleep_s = json["sleep"].toDouble();
    }
    if (json.contains("read")) {
        readScpi = json["read"].toString();
    }

    return true;
}

bool operator==(const DmmDriver &left, const DmmDriver &right) {
    if (left.resourceString != right.resourceString)
        return false;
    if (left.supply_V != right.supply_V)
        return false;
    if (left.measurementType != right.measurementType)
        return false;
    if (left.resistor_Ohms != right.resistor_Ohms)
        return false;
    if (left.setupScpi != right.setupScpi)
        return false;
    if (left.pointsScpi != right.pointsScpi)
        return false;
    if (left.startScpi != right.startScpi)
        return false;
    if (left.sleep_s != right.sleep_s)
        return false;
    if (left.readScpi != right.readScpi)
        return false;

    // Everything is the same
    return true;
}
bool operator!=(const DmmDriver &left, const DmmDriver &right) {
    return !(left == right);
}
