#include "measurepae.h"


// Project
#include "processtrace.h"
#include "tracesettings.h"

// RsaToolbox
#include <General.h>
#include <VnaChannel.h>
using namespace RsaToolbox;

// Qt
#include <QDebug>

// stdlib
#include <algorithm>


MeasurePAE::MeasurePAE(QObject *parent) :
    QObject(parent),
    _vna(0),
    _calculation(Calculation::powerAddedEfficiency)
{

}

MeasurePAE::~MeasurePAE()
{

}

void MeasurePAE::setVna(Vna *vna) {
    _vna = vna;
}
void MeasurePAE::setCalculation(Calculation calculation) {
    _calculation = calculation;
}

bool MeasurePAE::hasAcceptableInput(QString &message) {
    message.clear();
    if (!hasAcceptableTraceInput(message))
        return false;
    if (!hasAcceptableStageInput(message))
        return false;

    return true;
}

bool MeasurePAE::hasAcceptableTraceInput(QString &message) {
    message.clear();

    bool isTraces = false;
    if (isInputTrace() && isOutputTrace()) {
        isTraces = true;
    }
    if (isInputTrace() && isGainTrace()) {
        isTraces = true;
    }
    if (isGainTrace() && isOutputTrace()) {
        isTraces = true;
    }

    // Only need output power for de
    if (isDrainEfficiency() && isOutputTrace()) {
        isTraces = true;
    }

    if (!isTraces) {
        if (isDrainEfficiency()) {
            message = "*cannot calculate DE with traces given";
            return false;
        }
        else {
            message = "*cannot calculate PAE with traces given";
            return false;
        }
    }

    if (!isOneChannel()) {
        message = "*traces must be in same channel";
        return false;
    }

    return true;
}
void MeasurePAE::setInputTrace(const QString &name) {
    _inputTrace = name;
}
void MeasurePAE::setGainTrace(const QString &name) {
    _gainTrace = name;
}
void MeasurePAE::setOutputTrace(const QString &name) {
    _outputTrace = name;
}

bool MeasurePAE::isOneChannel() const {
    QVector<uint> channels;
    if (isInputTrace()) {
        channels << inputTraceChannel();
    }
    if (isGainTrace()) {
        channels << gainTraceChannel();
    }
    if (isOutputTrace()) {
        channels << outputTraceChannel();
    }

    // No traces
    if (channels.isEmpty()) {
        return false;
    }

    // Different channels
    const int first = channels.first();
    if (channels.size() != channels.count(first)) {
        return false;
    }

    return true;
}
uint MeasurePAE::channel() const {
    if (!isOneChannel())
        return 0;

    if (isInputTrace()) {
        return inputTraceChannel();
    }
    if (isGainTrace()) {
        return gainTraceChannel();
    }
    if (isOutputTrace()) {
        return outputTraceChannel();
    }

    return 0;
}

uint MeasurePAE::sweepPoints() const {
    return _vna->channel(channel()).linearSweep().points();
}
QVector<uint> MeasurePAE::sourcePorts() const {
    QVector<uint> result;

    VnaChannel ch = _vna->channel(channel());
    foreach (QString name, ch.traces()) {
        VnaTrace t = _vna->trace(name);
        if (t.isNetworkParameter()) {
            NetworkParameter param;
            uint output, input;
            t.networkParameter(param, output, input);
            if (!result.contains(input)) {
                result << input;
            }
        }
        if (t.isWaveQuantity()) {
            WaveQuantity wave;
            uint wavePort, sourcePort;
            t.waveQuantity(wave, wavePort, sourcePort);
            if (!result.contains(sourcePort)) {
                result << sourcePort;
            }
        }
        // ... other trace types?
    }
    std::sort(result.begin(), result.end());
    return result;
}

bool MeasurePAE::hasAcceptableStageInput(QString &message) {
    message.clear();
    return _controller.hasAcceptableStageInput(message);
}
void MeasurePAE::setStages(const QVector<StageSettings> &stages) {
    _stages = stages;
    _controller.setStages(stages);
}

QVector<StageResult> MeasurePAE::stageResults() const {
    if (!_results.isEmpty()) {
        return _results;
    }
    else {
        _results = _controller.readResults();
        return _results;
    }
}
QRowVector MeasurePAE::dcPower_W() const {
    const QVector<StageResult> results = stageResults();
    QRowVector power = results[0].power_W();
    for (int i = 1; i < results.size(); i++) {
        power = add(power, results[i].power_W());
    }
    return power;
}
QRowVector MeasurePAE::efficiency_pct() const {
    QRowVector numerator;
    if (isPowerAddedEfficiency()) {
        numerator = subtract(pout_W(), pin_W());
    }
    else {
        numerator = pout_W();
    }
    QRowVector efficiency = divideEach(numerator, dcPower_W());
    return multiply(efficiency, 100.0);
}

void MeasurePAE::run() {
    _results.clear();

    QString msg;
    if (!hasAcceptableInput(msg)) {
        emit error(msg);
        return;
    }
    if (!_controller.isConnected(msg)) {
        emit error(msg);
        return;
    }

    _controller.setup();
    _controller.start();
    sweepVna();
    display();
}

void MeasurePAE::sweepVna() {
    VnaChannel ch = _vna->channel(channel());
    uint timeout_ms = 2 * ch.sweepTime_ms() + 5000;
    ch.startSweep();
    _vna->pause(timeout_ms);
}

bool MeasurePAE::isPowerAddedEfficiency() const {
    return _calculation == Calculation::powerAddedEfficiency;
}
bool MeasurePAE::isDrainEfficiency() const {
    return _calculation == Calculation::drainEfficiency;
}

bool MeasurePAE::isInputTrace() const {
    return !_inputTrace.isEmpty();
}
bool MeasurePAE::isGainTrace() const {
    return !_gainTrace.isEmpty();
}
bool MeasurePAE::isOutputTrace() const {
    return !_outputTrace.isEmpty();
}

uint MeasurePAE::inputTraceChannel() const {
    return _vna->trace(_inputTrace).channel();
}
uint MeasurePAE::gainTraceChannel() const {
    return _vna->trace(_gainTrace).channel();
}
uint MeasurePAE::outputTraceChannel() const {
    return _vna->trace(_outputTrace).channel();
}

QRowVector MeasurePAE::pin_W() const {
    if (isInputTrace()) {
        return readPin_W();
    }
    return calculatePin_W();
}
QRowVector MeasurePAE::pout_W() const {
    if (isOutputTrace()) {
        return readPout_W();
    }
    return calculatePout_W();
}

QRowVector MeasurePAE::readPout_W() const {
    return read_W(_outputTrace);
}
QRowVector MeasurePAE::readGain_U() const {
    return read_W(_gainTrace);
}
QRowVector MeasurePAE::readPin_W() const {
    return read_W(_inputTrace);
}
QRowVector MeasurePAE::read_W(const QString &name) const {
    VnaTrace t = _vna->trace(name);
    TraceFormat format = t.format();
    t.setFormat(TraceFormat::Magnitude);
    QRowVector result;
    t.y(result);
    t.setFormat(format);
    return result;
}
QRowVector MeasurePAE::calculatePin_W() const {
    const QRowVector gain = readGain_U();
    return divideEach(readPout_W(), multiplyEach(gain, gain));
}
QRowVector MeasurePAE::calculatePout_W() const {
    const QRowVector gain = readGain_U();
    return multiplyEach(readPin_W(), multiplyEach(gain, gain));
}


uint MeasurePAE::nextDiagram() const {
    QVector<uint> diagrams = _vna->diagrams();
    if (diagrams.isEmpty())
        return 1;

    return max(diagrams) + 1;
}
uint MeasurePAE::outputPort() const {
    if (isOutputTrace()) {
        WaveQuantity wave;
        uint outputPort, sourcePort;
        _vna->trace(_outputTrace).waveQuantity(wave, outputPort, sourcePort);
        return outputPort;
    }
    else {
        NetworkParameter parameter;
        uint outputPort, inputPort;
        _vna->trace(_gainTrace).networkParameter(parameter, outputPort, inputPort);
        return outputPort;
    }
}
uint MeasurePAE::inputPort() const {
    if (isInputTrace()) {
        WaveQuantity wave;
        uint inputPort, sourcePort;
        _vna->trace(_inputTrace).waveQuantity(wave, inputPort, sourcePort);
        return inputPort;
    }
    else {
        NetworkParameter parameter;
        uint outputPort, inputPort;
        _vna->trace(_gainTrace).networkParameter(parameter, outputPort, inputPort);
        return inputPort;
    }
}
void MeasurePAE::display() const {
    qDebug() << "Displaying traces";
    uint channel    = this->channel();
    uint diagram    = nextDiagram();
    uint outputPort = this->outputPort();
    uint inputPort  = this->inputPort();

    // Stage current traces
    QVector<StageResult> results = stageResults();
    for (int i = 0; i < _stages.size(); i++) {
        qDebug() << "  displaying stage " << i;
        TraceSettings settings;
        settings.name       = _stages[i].name;
        settings.channel    = channel;
        settings.diagram    = diagram;
        settings.outputPort = outputPort;
        settings.inputPort  = inputPort;
        settings.data       = _results[i].current_A();
        ProcessTrace(settings, _vna);
    }

    // PAE/DE
    qDebug() << "  displaying PAE/DE";
    TraceSettings settings;
    if (isPowerAddedEfficiency()) {
        settings.name = "PAE";
    }
    else {
        settings.name = "DE";
    }
    settings.channel = channel;
    settings.diagram = diagram;
    settings.outputPort = outputPort;
    settings.inputPort = inputPort;
    settings.data = efficiency_pct();
    ProcessTrace(settings, _vna);
}
