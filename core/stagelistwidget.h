#ifndef STAGELISTWIDGET_H
#define STAGELISTWIDGET_H


// Project
#include "stagesettingsmodel.h"

// Qt
#include <QModelIndex>
#include <QScopedPointer>
#include <QWidget>


namespace Ui {
class StageListWidget;
}

class StageListWidget : public QWidget
{
    Q_OBJECT

public:
    explicit StageListWidget(QWidget *parent = 0);
    ~StageListWidget();

private slots:
    void tableDoubleClicked(const QModelIndex &index);
    void addStage();
    void removeStage();
    void moveStageUp();
    void moveStageDown();

private:
    Ui::StageListWidget *ui;

    QScopedPointer<StageSettingsModel> _model;
};

#endif // STAGELISTWIDGET_H
