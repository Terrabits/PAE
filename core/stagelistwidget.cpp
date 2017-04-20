#include "stagelistwidget.h"
#include "ui_stagelistwidget.h"


// Project
#include "stagedialog.h"

// Qt
#include <QApplication>
#include <QVector>


StageListWidget::StageListWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::StageListWidget)
{
    ui->setupUi(this);

    _model.reset(new StageSettingsModel);

    QVector<StageSettings> settings(4);
    _model->setSettings(settings);
    ui->stages->setModel(_model.data());

    connect(ui->stages, SIGNAL(doubleClicked(QModelIndex)),
            this, SLOT(tableDoubleClicked(QModelIndex)));

    ui->panel->setModel(_model.data());
    ui->panel->setSelectionModel(ui->stages->selectionModel());
}

StageListWidget::~StageListWidget()
{
    delete ui;
}

void StageListWidget::tableDoubleClicked(const QModelIndex &index) {
    if (!index.isValid())
        return;

    const int row = index.row();
    QVector<StageSettings> settings = _model->settings();
    StageDialog dialog(QApplication::activeWindow());
    dialog.setSettings(settings[row]);
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
