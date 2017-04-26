#ifndef STAGERESULT_H
#define STAGERESULT_H


// Project
#include "stagesettings.h"

// RsaToolbox
#include <Definitions.h>

// Qt
#include <QVector>


class StageResult
{
public:
    StageResult();
    StageResult(const RsaToolbox::QRowVector &dmm_V, const StageSettings &settings);
    ~StageResult();

    RsaToolbox::QRowVector current_A() const;
    RsaToolbox::QRowVector voltage_V() const;
    RsaToolbox::QRowVector power_W  () const;

private:
    StageSettings _settings;
    RsaToolbox::QRowVector _current_A;
    RsaToolbox::QRowVector _voltage_V;
};


#endif // STAGERESULT_H
