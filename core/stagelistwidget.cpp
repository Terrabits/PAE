#include "stagelistwidget.h"
#include "ui_stagelistwidget.h"


// Project
#include "dmmcontroller.h"
#include "stagedialog.h"

// RsaToolbox
using namespace RsaToolbox;

// Qt
#include <QApplication>
#include <QDebug>
#include <QVector>


StageListWidget::StageListWidget(QWidget *parent) :
    QWidget(parent),
    ui(new ::Ui::StageListWidget),
    _keys(0)
{
    ui->setupUi(this);

    bool isBlocked = blockSignals(true);

    ui->delay->setValue(0);
    ui->delay->setMinimum(0);
    ui->delay->interpretMKeyAsMilli();
    ui->delay->setUnits(Units::Seconds);

    _model.reset(new StageSettingsModel);
    ui->stages->setModel(_model.data());

    connect(ui->stages, SIGNAL(doubleClicked(QModelIndex)),
            this, SLOT(tableDoubleClicked(QModelIndex)));

    ui->panel->setModel(_model.data());
    ui->panel->setSelectionModel(ui->stages->selectionModel());

    blockSignals(isBlocked);
}

StageListWidget::~StageListWidget()
{
    delete ui;
}

bool StageListWidget::hasAcceptableInput() const {
    QString msg;
    return hasAcceptableInput(msg);
}
bool StageListWidget::hasAcceptableInput(QString &message) const {
    message.clear();

    DmmController controller;
    controller.setStages(stages());
    return controller.hasAcceptableStageInput(message);
}

double StageListWidget::delay_s() const {
    return ui->delay->value();
}
void StageListWidget::setDelay(double value, SiPrefix prefix) {
    ui->delay->setValue(value, prefix);
}

void StageListWidget::addStage(int row) {
    if (!_model) {
        return;
    }

    _model->insertRow(row);
    const QModelIndex i = _model->index(row, 0);
    ui->stages->selectRow(row);
    ui->stages->setCurrentIndex(i);
}

QVector<StageSettings> StageListWidget::stages() const {
    return _model->settings();
}

bool StageListWidget::isKeys() const {
    return _keys != 0;
}
void StageListWidget::loadKeys() {
    if (!isKeys()) {
        return;
    }

    QVector<StageSettings> settings;
    if (_keys->exists("PAE_STAGES")) {
        _keys->get("PAE_STAGES", settings);
        _model->setSettings(settings);

        const int   lastRow = settings.size() - 1;
        const QModelIndex i = _model->index(lastRow, 0);
        ui->stages->setCurrentIndex(i);
    }
    if (_keys->exists("DMM_DELAY")) {
        double value;
        _keys->get("DMM_DELAY", value);
        ui->delay->setValue(value);
    }
}
void StageListWidget::saveKeys() const {
    if (!isKeys())
        return;
    _keys->set("PAE_STAGES", stages());
}
void StageListWidget::setKeys(RsaToolbox::Keys *keys) {
    _keys = keys;
}

void StageListWidget::tableDoubleClicked(const QModelIndex &index) {
    if (!index.isValid())
        return;

    const int row    = index.row();
    const int column = index.column();
    QVector<StageSettings> settings = _model->settings();
    StageDialog dialog(_keys, QApplication::activeWindow());
    dialog.setSettings(settings[row]);
    switch (column) {
    case StageSettingsModel::Column::name:
        dialog.highlightName();
        break;
    case StageSettingsModel::Column::connectionType:
        dialog.highlightConnectionType();
        break;
    case StageSettingsModel::Column::address:
        dialog.highlightAddress();
        break;
    case StageSettingsModel::Column::driver:
        dialog.highlightDriver();
        break;
    case StageSettingsModel::Column::powerSupply:
        dialog.highlightVoltage();
        break;
    case StageSettingsModel::Column::shuntResistor:
        dialog.highlightResistance();
        break;
    }
    dialog.exec();

    if (dialog.result() != QDialog::Accepted) {
        return;
    }

    // Handle changes
    settings[row] = dialog.settings();
    _model->setSettings(settings);
}
