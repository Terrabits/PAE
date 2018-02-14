#include "mainwindow.h"
#include "ui_mainwindow.h"


// Project
#include "measurepae.h"

// RsaToolbox
using namespace RsaToolbox;

// Qt
#include <QCloseEvent>
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

    _thread->quit();
    _thread->deleteLater();
    _thread.take();
}

void MainWindow::closeEvent(QCloseEvent *event) {
    saveKeys();
    event->accept();
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

    _measure.reset(new MeasurePAE);
    _measure->setVna(_vna);
    _measure->setCalculation(MeasurePAE::Calculation(calculation()));
    _measure->setInputTrace(ui->traces->inputTrace());
    _measure->setGainTrace(ui->traces->gainTrace());
    _measure->setOutputTrace(ui->traces->outputTrace());
    _measure->setStages(ui->stageList->stages());

    QString msg;
    if (!_measure->hasAcceptableTraceInput(msg)) {
        showError(msg);
        ui->traces->setFocus();
        return;
    }
    if (!_measure->hasAcceptableStageInput(msg)) {
        showError(msg);
        ui->stageList->setFocus();
        return;
    }
    if (!_measure->hasAcceptableInput(msg)) {
        showError(msg);
        return;
    }

    _isError = false;
    _measure->moveToThread(_thread.data());
    connectMeasure();
    QMetaObject::invokeMethod(_measure.data(), "run");
}
void MainWindow::disableInputs() {
    this->setDisabled(true);
}
void MainWindow::enableInputs() {
    this->setEnabled(true);
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

    if (ui->stageList->stages().isEmpty()) {
        ui->stageList->addStage();
    }

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

    _isError = false;
    _thread.reset(new QThread);
    _thread->start();
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
        ui->paeRadio->setChecked(true);
        ui->deRadio->setChecked(false);

    }
    else {
        ui->deRadio->setChecked(true);
        ui->paeRadio->setChecked(false);
    }
}

void MainWindow::connectMeasure() {
    connect(_measure.data(), SIGNAL(started()),
            this,            SLOT  (disableInputs()));
    connect(_measure.data(), SIGNAL(error(QString)),
            this,            SLOT  (error(QString)));
    connect(_measure.data(), SIGNAL(finished()),
            this,            SLOT  (enableInputs()));
    connect(_measure.data(), SIGNAL(finished()),
            this,            SLOT  (disconnectMeasure()));
    connect(_measure.data(), SIGNAL(finished()),
            this,            SLOT  (measurementComplete()));
}
void MainWindow::disconnectMeasure() {
    _measure->disconnect();
}
void MainWindow::error(const QString &message) {
    _isError = true;
    showError(message);
}
void MainWindow::measurementComplete() {
    if (_isError) {
        return;
    }
    ui->error->showMessage("Measurement complete.", Qt::darkGreen);
}
