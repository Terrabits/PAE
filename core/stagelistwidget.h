#ifndef STAGELISTWIDGET_H
#define STAGELISTWIDGET_H


// Project
#include "stagesettings.h"
#include "stagesettingsmodel.h"

// RsaToolbox
#include <Keys.h>

// Qt
#include <QModelIndex>
#include <QScopedPointer>
#include <QString>
#include <QVector>
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

    void setKeys(RsaToolbox::Keys *keys);

    bool hasAcceptableInput() const;
    bool hasAcceptableInput(QString &message) const;
    QVector<StageSettings> stages() const;

    bool isKeys() const;
    void loadKeys();
    void saveKeys() const;

signals:
    void error(const QString &message);

private slots:
    void tableDoubleClicked(const QModelIndex &index);
    void addStage();
    void removeStage();
    void moveStageUp();
    void moveStageDown();

private:
    Ui::StageListWidget *ui;
    RsaToolbox::Keys *_keys;

    QScopedPointer<StageSettingsModel> _model;
};

#endif // STAGELISTWIDGET_H
