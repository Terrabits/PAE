#ifndef TRACESETTINGS_H
#define TRACESETTINGS_H


// RsaToolbox
#include <Definitions.h>

// Qt
#include <Qt>


class TraceSettings
{
public:
    TraceSettings();

    QString name;
    uint channel;
    uint diagram;
    uint outputPort;
    uint inputPort;
    RsaToolbox::QRowVector data;
};

#endif // TRACESETTINGS_H
