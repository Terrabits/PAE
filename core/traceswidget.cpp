#include "traceswidget.h"
#include "ui_traceswidget.h"


// Project
#include "measurepae.h"

// RsaToolbox
using namespace RsaToolbox;

// Keys
const QString INPUT_TRACE("PAE_INPUT_TRACE");
const QString GAIN_TRACE("PAE_GAIN_TRACE");
const QString OUTPUT_TRACE("PAE_OUTPUT_TRACE");


TracesWidget::TracesWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TracesWidget),
    _vna(0),
    _keys(0)
{
    ui->setupUi(this);
}

TracesWidget::~TracesWidget()
{
    delete ui;
}

void TracesWidget::setAccess(Vna *vna, Keys *keys) {
    _vna  = vna;
    _keys = keys;

    if (isVna()) {
        updateTraces();
    }
}

bool TracesWidget::hasAcceptableInput(Calculation calculation, QString &message) {
    message.clear();

    MeasurePAE pae;
    pae.setVna(_vna);
    if (calculation == Calculation::powerAddedEfficiency) {
        pae.setCalculation(MeasurePAE::Calculation::powerAddedEfficiency);
    }
    else {
        pae.setCalculation(MeasurePAE::Calculation::drainEfficiency);
    }
    if (isInputTrace())
        pae.setInputTrace(inputTrace());
    if (isGainTrace())
        pae.setGainTrace(gainTrace());
    if (isOutputTrace())
        pae.setOutputTrace(outputTrace());
    return pae.hasAcceptableTraceInput(message);
}

bool TracesWidget::isInputTrace() const {
    return ui->inputPowerTrace->currentIndex() != -1;
}
bool TracesWidget::isGainTrace() const {
    return ui->gainTrace->currentIndex() != -1;
}
bool TracesWidget::isOutputTrace() const {
    return ui->outputPowerTrace->currentIndex() != -1;
}
QString TracesWidget::inputTrace() const {
    if (isInputTrace())
        return ui->inputPowerTrace->currentText();
    else
        return QString();
}
QString TracesWidget::gainTrace() const {
    if (isGainTrace())
        return ui->gainTrace->currentText();
    else
        return QString();
}
QString TracesWidget::outputTrace() const {
    if (isOutputTrace())
        return ui->outputPowerTrace->currentText();
    else
        return QString();
}

void TracesWidget::on_clearInputPower_clicked() {
    ui->inputPowerTrace->setCurrentIndex(-1);
}
void TracesWidget::on_clearGain_clicked() {
    ui->gainTrace->setCurrentIndex(-1);
}
void TracesWidget::on_clearOutputPower_clicked() {
    ui->outputPowerTrace->setCurrentIndex(-1);
}

bool TracesWidget::isVna() const {
    return _vna != 0;
}
bool TracesWidget::isKeys() const {
    return _keys != 0;
}
void TracesWidget::loadKeys() {
    if (!isKeys())
        return;

    QString name;
    if (_keys->exists(INPUT_TRACE)) {
        _keys->get(INPUT_TRACE, name);
        setInputTrace(name);
    }
    if (_keys->exists(GAIN_TRACE)) {
        _keys->get(GAIN_TRACE, name);
        setGainTrace(name);
    }
    if (_keys->exists(OUTPUT_TRACE)) {
        _keys->get(OUTPUT_TRACE, name);
        setOutputTrace(name);
    }
}
void TracesWidget::saveKeys() const {
    if (!isKeys())
        return;

    if (isInputTrace()) {
        _keys->set(INPUT_TRACE,  inputTrace());
    }
    else {
        _keys->remove(INPUT_TRACE);
    }
    if (isGainTrace()) {
        _keys->set(GAIN_TRACE,   gainTrace());
    }
    else {
        _keys->remove(GAIN_TRACE);
    }
    if (isOutputTrace()) {
        _keys->set(OUTPUT_TRACE, outputTrace());
    }
    else {
        _keys->remove(OUTPUT_TRACE);
    }
}

void TracesWidget::clearInputTrace() {
    ui->inputPowerTrace->setCurrentIndex(-1);
}
void TracesWidget::clearGainTrace() {
    ui->gainTrace->setCurrentIndex(-1);
}
void TracesWidget::clearOutputTrace() {
    ui->outputPowerTrace->setCurrentIndex(-1);
}
void TracesWidget::clearTraces() {
    clearInputTrace();
    clearGainTrace();
    clearOutputTrace();
}

void TracesWidget::updateTraces() {
    ui->inputPowerTrace->clear();
    ui->gainTrace->clear();
    ui->outputPowerTrace->clear();

    if (!isVna()) {
        return;
    }

    QStringList traces = _vna->traces();

    QStringList waveTraces;
    QStringList gainTraces;
    foreach(const QString &t, traces) {
        VnaTrace trace = _vna->trace(t);
        if (trace.isWaveQuantity()) {
            waveTraces << t;
        }
        if (trace.isNetworkParameter()) {
            NetworkParameter param;
            uint inputPort, outputPort;
            trace.networkParameter(param, outputPort, inputPort);
            if (param == NetworkParameter::S && inputPort != outputPort) {
                gainTraces << t;
            }
        }
    }

    ui->inputPowerTrace->insertItems(0, waveTraces);
    ui->gainTrace->insertItems(0, gainTraces);
    ui->outputPowerTrace->insertItems(0, waveTraces);
    clearTraces();
}
void TracesWidget::setInputTrace(const QString &name) {
    setCombo(ui->inputPowerTrace, name);
}
void TracesWidget::setGainTrace(const QString &name) {
    setCombo(ui->gainTrace, name);
}
void TracesWidget::setOutputTrace(const QString &name) {
    setCombo(ui->outputPowerTrace, name);
}
void TracesWidget::setCombo(QComboBox *combo, const QString &name) {
    bool isBlocked = combo->blockSignals(true);
    const int count = combo->count();
    for (int i = 0; i < count; i++) {
        if (name.compare(combo->itemText(i), Qt::CaseInsensitive) == 0) {
            combo->setCurrentIndex(i);
            return;
        }
    }
    combo->setCurrentIndex(-1);
    combo->blockSignals(isBlocked);
}
