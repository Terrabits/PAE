#include "stagelistwidget.h"
#include "ui_stagelistwidget.h"


// Project
#include "dmmcontroller.h"
#include "stagedialog.h"

// Qt
#include <QApplication>
#include <QDebug>
#include <QVector>


StageListWidget::StageListWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::StageListWidget),
    _keys(0)
{
    ui->setupUi(this);

    bool isBlocked = blockSignals(true);

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
QVector<StageSettings> StageListWidget::stages() const {
    return _model->settings();
}

bool StageListWidget::isKeys() const {
    return _keys != 0;
}
void StageListWidget::loadKeys() {
    QVector<StageSettings> settings;
    if (_keys->exists("PAE_STAGES")) {
        _keys->get("PAE_STAGES", settings);
        _model->setSettings(settings);
    }
}
void StageListWidget::saveKeys() const {
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
    StageDialog dialog(QApplication::activeWindow());
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

    if (dialog.result() != QDialog::Accepted)
        return;

    // Handle changes
    settings[row] = dialog.settings();
    _model->setSettings(settings);
}
void StageListWidget::addStage() {
    QString name = "Stage %1";
    name = name.arg(_model->settings().size()+1);

    StageDialog dialog(this);
    dialog.setSettings(StageSettings(name));
    dialog.exec();

    if (dialog.result() != QDialog::Accepted)
        return;

    QVector<StageSettings> settings = _model->settings();
    settings << dialog.settings();
    _model->setSettings(settings);
}
void StageListWidget::removeStage() {

}
void StageListWidget::moveStageUp() {

}
void StageListWidget::moveStageDown() {

}
