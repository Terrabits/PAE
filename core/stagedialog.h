#ifndef STAGEDIALOG_H
#define STAGEDIALOG_H


// Project
#include "stagesettings.h"

// RsaToolbox
#include <Keys.h>
#include <LastPath.h>

// Qt
#include <QDialog>


namespace Ui {
class StageDialog;
}

class StageDialog : public QDialog
{
    Q_OBJECT

public:
    explicit StageDialog(RsaToolbox::Keys *keys, QWidget *parent = 0);
    ~StageDialog();

    void highlightName();
    void highlightConnectionType();
    void highlightAddress();
    void highlightDriver();
    void highlightVoltage();
    void highlightResistance();

    StageSettings settings() const;
    void setSettings(const StageSettings &settings);

private slots:
    void getDriver();

private:
    Ui::StageDialog *ui;
    RsaToolbox::LastPath _lastPath;

    void setDriver(const QString &driver);
    QString _driver;
};

#endif // STAGEDIALOG_H
