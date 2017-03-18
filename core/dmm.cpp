#include "dmm.h"

// RsaToolbox
using namespace RsaToolbox;

Dmm::Dmm(QObject *parent) :
    GenericInstrument(parent)
{

}
Dmm::Dmm(GenericBus *bus, QObject *parent) :
    GenericInstrument(bus, parent)
{

}
Dmm::Dmm(ConnectionType type, QString address, QObject *parent) :
    GenericInstrument(type, address, parent)
{

}

Dmm::Dmm(ConnectionType type, QString address, const DmmDriver &driver, QObject *parent) :
    GenericInstrument(type, address, parent),
    _driver(driver)
{

}
