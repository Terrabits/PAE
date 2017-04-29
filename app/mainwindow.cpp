#include "mainwindow.h"
#include "ui_mainwindow.h"


// Project
#include "measurepae.h"

// RsaToolbox
using namespace RsaToolbox;

// Qt
#include <QDebug>
#include <QPushButton>


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    _vna(0),
    _keys(0)
{
    ui->setupUi(this);
    init();
}
MainWindow::MainWindow(RsaToolbox::Vna *vna, RsaToolbox::Keys *keys, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    _vna(vna),
    _keys(keys)
{
    ui->setupUi(this);
    init();
}

MainWindow::~MainWindow()
{
    delete ui;
}

bool MainWindow::close() {
    saveKeys();
    return QMainWindow::close();
}

void MainWindow::showError(const QString &message) {
    ui->error->showMessage(message);
    shake(this);
}

void MainWindow::run() {
    ui->error->clearMessage();
    if (!hasAcceptableInput()) {
        return;
    }

    MeasurePAE measure;
    measure.setVna(_vna);
    measure.setCalculation(MeasurePAE::Calculation(calculation()));
    measure.setInputTrace(ui->traces->inputTrace());
    measure.setGainTrace(ui->traces->gainTrace());
    measure.setOutputTrace(ui->traces->outputTrace());
    measure.setStages(ui->stageList->stages());

    QString msg;
    if (!measure.hasAcceptableTraceInput(msg)) {
        showError(msg);
        ui->traces->setFocus();
        return;
    }
    if (!measure.hasAcceptableStageInput(msg)) {
        showError(msg);
        ui->stageList->setFocus();
        return;
    }
    if (!measure.hasAcceptableInput(msg)) {
        showError(msg);
        return;
    }

    connect(&measure, SIGNAL(error(QString)),
            this, SLOT(showError(QString)));
    measure.run();
    disconnect(&measure, SIGNAL(error(QString)),
            this, SLOT(showError(QString)));
}

void MainWindow::loadKeys() {
    if (!_keys) {
        return;
    }

    if (_keys->exists("PAE_CALC")) {
        quint32 calc;
        _keys->get("PAE_CALC", calc);
        setCalculation(TracesWidget::Calculation(calc));
    }
    ui->traces->loadKeys();
    ui->stageList->loadKeys();
}
void MainWindow::saveKeys() {
    if (!_keys) {
        return;
    }
    _keys->set("PAE_CALC", quint32(calculation()));
    ui->traces->saveKeys();
    ui->stageList->saveKeys();
}

void MainWindow::init() {
    Q_INIT_RESOURCE(core);
    ui->traces->setAccess(_vna, _keys);
    ui->stageList->setKeys(_keys);
    loadKeys();

    connect(ui->traces, SIGNAL(error(QString)),
            this, SLOT(showError(QString)));
    connect(ui->stageList, SIGNAL(error(QString)),
            this, SLOT(showError(QString)));

    connect(ui->buttonBox, SIGNAL(rejected()),
            this, SLOT(close()));
    connect(ui->buttonBox, SIGNAL(accepted()),
            this, SLOT(run()));

    QPushButton *okButton = ui->buttonBox->button(QDialogButtonBox::Ok);
    if (okButton) {
        okButton->setText("Run");
    }


}

bool MainWindow::hasAcceptableInput() {
    QString msg;
    if (!ui->traces->hasAcceptableInput(calculation(), msg)) {
        ui->traces->setFocus();
        showError(msg);
        return false;
    }
    if (!ui->stageList->hasAcceptableInput(msg)) {
        ui->stageList->setFocus();
        showError(msg);
        return false;
    }
    return true;
}
TracesWidget::Calculation MainWindow::calculation() const {
    if (ui->paeRadio->isChecked()) {
        return TracesWidget::Calculation::powerAddedEfficiency;
    }
    else {
        return TracesWidget::Calculation::drainEfficiency;
    }
}
void MainWindow::setCalculation(TracesWidget::Calculation calc) {
    if (calc == TracesWidget::Calculation::powerAddedEfficiency) {
        ui->deRadio->setChecked(false);
        ui->paeRadio->setChecked(true);
    }
    else {
        ui->paeRadio->setChecked(false);
        ui->deRadio->setChecked(true);
    }
}
