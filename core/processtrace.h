#ifndef PROCESSTRACE_H
#define PROCESSTRACE_H


// Project
#include "tracesettings.h"

// RsaToolbox
#include <Definitions.h>
#include <General.h>
#include <Vna.h>

// Qt
#include <Qt>
#include <QString>


class ProcessTrace
{
public:
    ProcessTrace(TraceSettings settings, RsaToolbox::Vna *vna);
    ~ProcessTrace();

private:
    TraceSettings    _settings;
    RsaToolbox::Vna *_vna;

    QString dataTraceName();
    QString memoryTraceName();

    bool isDataTrace();
    bool isMemoryTrace();

    RsaToolbox::VnaTrace dataTrace();
    RsaToolbox::VnaTrace memoryTrace();

    bool isPreexistingTrace();
    void createTrace();
    void updateTrace();

    void debugPrint(const QString &name);
};

#endif // PROCESSTRACE_H
