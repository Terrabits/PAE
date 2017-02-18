#include "keysightdmm.h"

// RsaToolbox
#include <General.h>
using namespace RsaToolbox;

// Qt
#include <QDebug>
#include <QThread>

KeysightDmm::KeysightDmm(QObject *parent) :
    GenericInstrument(parent)
{

}

KeysightDmm::KeysightDmm(GenericBus *bus, QObject *parent) :
    GenericInstrument(bus, parent)
{

}

KeysightDmm::KeysightDmm(ConnectionType type, QString address, QObject *parent) :
    GenericInstrument(type, address, parent)
{

}

// Set
void KeysightDmm::setRange(const QString &value) {
    QString scpi = "SENS:VOLT:DC:RANG %1\n";
    scpi         = scpi.arg(value);
    write(scpi);
}
void KeysightDmm::setNplc(double value) {
    QString scpi = "SENS:VOLT:DC:NPLC %1\n";
    scpi         = scpi.arg(value);
    write(scpi);
}
void KeysightDmm::setTriggerSource(const QString &value) {
    QString scpi = "TRIG:SOUR %1\n";
    scpi         = scpi.arg(value);
    write(scpi);
}
void KeysightDmm::setTriggerDelay(double value) {
    QString scpi = "TRIG:DEL %1\n";
    scpi         = scpi.arg(value);
    write(scpi);
}
void KeysightDmm::setTriggerCount(uint value) {
    QString scpi = "TRIG:COUN %1\n";
    scpi         = scpi.arg(value);
    write(scpi);
}
void KeysightDmm::setTriggerSlope(const QString &value) {
    QString scpi = "TRIG:SLOP %1\n";
    scpi         = scpi.arg(value);
    write(scpi);
}
void KeysightDmm::setSampleSource(const QString &value) {
    QString scpi = "SAMP:SOUR %1\n";
    scpi         = scpi.arg(value);
    write(scpi);
}
void KeysightDmm::setSampleCount(uint value) {
    QString scpi = "SAMP:COUN %1\n";
    scpi         = scpi.arg(value);
    write(scpi);
}
void KeysightDmm::setDataFormat(const QString &value) {
    QString scpi = "FORM:DATA %1\n";
    scpi         = scpi.arg(value);
    write(scpi);
}
void KeysightDmm::setBitOrder(const QString &value) {
    QString scpi = "FORM:BORD %1\n";
    scpi         = scpi.arg(value);
    write(scpi);
}

void KeysightDmm::startMeasurement() {
    write("INIT\n");
    QThread::msleep(500);
}
void KeysightDmm::abort() {
    write("ABOR");
}
QRowVector KeysightDmm::results(uint points) {
    QString data = query("FETC?\n", 20 * points);
    return parseQRowVector(data);
}

// Errors
bool KeysightDmm::isError() {
    QStringList messages;
    return errors(messages);
}

bool KeysightDmm::nextError(QString &message) {
    int code = 0;
    message.clear();

    QString result = query(":SYST:ERR?\n").trimmed();
    int sep = result.indexOf(",");
    code = result.mid(0, sep).toInt();
    message = result.mid(sep+1);
    message.remove(0,1);
    message.chop(1);
    return code != 0;
}
bool KeysightDmm::errors(QStringList &messages) {
    messages.clear();

    QString msg;
    while (nextError(msg)) {
        messages << msg;
    }
    return !messages.isEmpty();
}
