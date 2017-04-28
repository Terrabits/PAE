#include "traceswidget.h"
#include "ui_traceswidget.h"


// RsaToolbox
using namespace RsaToolbox;


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

void TracesWidget::setVna(RsaToolbox::Vna *vna) {
    _vna = vna;
}
void TracesWidget::setKeys(RsaToolbox::Keys *keys) {
    _keys = keys;
}

void TracesWidget::on_channel_currentIndexChanged(int index) {
    if (index == -1) {
        return;
    }

    const uint c = _channels[index];
    QStringList traces = _vna->channel(c).traces();

    QStringList waveTraces;
    QStringList gainTraces;
    foreach(const QString &t, traces) {
        VnaTrace trace = _vna->trace(t);
        if (trace.isWaveQuantity()) {
            waveTraces << t;
        }
        if (trace.isNetworkParameter()) {
            NetworkParameter param;
            BalancedPort inputPort, outputPort;
            trace.networkParameter(param, outputPort, inputPort);
            if (param == NetworkParameter::S && inputPort != outputPort) {
                gainTraces << t;
            }
        }
    }

    ui->inputPowerTrace->clear();
    ui->outputPowerTrace->clear();
    ui->inputPowerTrace->insertItems(0, waveTraces);
    ui->outputPowerTrace->insertItems(0, waveTraces);

    ui->gainTrace->clear();
    ui->gainTrace->insertItems(0, gainTraces);
}
