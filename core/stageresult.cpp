#include "stageresult.h"


// RsaToolbox
#include <General.h>
using namespace RsaToolbox;


StageResult::StageResult()
{

}
StageResult::StageResult(const RsaToolbox::QRowVector &dmm_V, const StageSettings &settings) {
    _settings = settings;
    _current_A = multiply(dmm_V, (1.0/settings.shuntResistor_ohms));
    _voltage_V = add(multiply(dmm_V, -1.0), settings.powerSupply_V);
}

StageResult::~StageResult()
{

}

QRowVector StageResult::current_A() const {
    return _current_A;
}
QRowVector StageResult::voltage_V() const {
    return _voltage_V;
}
QRowVector StageResult::power_W  () const {
    return multiplyEach(_current_A, _voltage_V);
}
