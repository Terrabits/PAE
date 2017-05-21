#include "ProcessTrace.h"


// RsaToolbox
using namespace RsaToolbox;

// Qt
#include <QDebug>

ProcessTrace::ProcessTrace(TraceSettings settings, Vna *vna) :
    _settings(settings),
    _vna(vna)
{
    _vna->settings().updateDisplay();
    _vna->settings().displayOn();
    if (isPreexistingTrace()) {
        updateTrace();
    }
    else {
        createTrace();
    }
}
ProcessTrace::~ProcessTrace() {

}

QString ProcessTrace::dataTraceName() {
    return "_" + _settings.name;
}
QString ProcessTrace::memoryTraceName() {
    return _settings.name;
}

bool ProcessTrace::isDataTrace() {
    return _vna->isTrace(dataTraceName());
}
bool ProcessTrace::isMemoryTrace() {
    return _vna->isTrace(memoryTraceName());
}

VnaTrace ProcessTrace::dataTrace() {
    return _vna->trace(dataTraceName());
}
VnaTrace ProcessTrace::memoryTrace() {
    return _vna->trace(memoryTraceName());
}

bool ProcessTrace::isPreexistingTrace() {
    if (!isDataTrace() || !isMemoryTrace()) {
        if (isDataTrace())
            _vna->deleteTrace(dataTraceName());
        if (isMemoryTrace())
            _vna->deleteTrace(memoryTraceName());
        return false;
    }
    else {
        return true;
    }
}

void ProcessTrace::createTrace() {
    const uint outputPort = _settings.outputPort;
    const uint inputPort  = _settings.inputPort;
    _vna->createTrace(dataTraceName(), _settings.channel);

    // set parameter
    VnaTrace _dataTrace = dataTrace();
    VnaTrace _memTrace  = memoryTrace();
    _dataTrace.setPae(outputPort, inputPort);

    _vna->createDiagram(_settings.diagram);
    _dataTrace.setDiagram(_settings.diagram);
    _dataTrace.toMemory(memoryTraceName());
    _memTrace.setDiagram(_settings.diagram);
    updateTrace();
    _dataTrace.hide();
}
void ProcessTrace::updateTrace() {
    const uint outputPort = _settings.outputPort;
    const uint inputPort  = _settings.inputPort;
    VnaTrace _dataTrace    = dataTrace();
    _dataTrace.setPae(outputPort, inputPort);
    _dataTrace.toMemory(memoryTraceName());

    VnaTrace memTrace = memoryTrace();
    memTrace.write(_settings.data);
}
