#ifndef MAINWINDOW_H
#define MAINWINDOW_H

// Project
#include "measurepae.h"
#include "traceswidget.h"

// RsaToolbox
#include <Keys.h>
#include <Shake.h>
#include <Vna.h>

// Qt
#include <QMainWindow>
#include <QScopedPointer>
#include <QThread>
#include <QVector>


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    MainWindow(RsaToolbox::Vna *vna, RsaToolbox::Keys *keys, QWidget *parent = 0);
    ~MainWindow();

private slots:
    void run();
    void disableInputs();
    void enableInputs();
    void connectMeasure();
    void disconnectMeasure();
    void error(const QString &message);
    void showError(const QString &message);
    void measurementComplete();

protected:
    virtual void closeEvent(QCloseEvent *event);

private:
    Ui::MainWindow *ui;

    RsaToolbox::Vna  *_vna;

    void loadKeys();
    void saveKeys();
    RsaToolbox::Keys *_keys;

    void init();

    bool hasAcceptableInput();

    TracesWidget::Calculation calculation() const;
    void setCalculation(TracesWidget::Calculation calc);

    bool _isError;
    QScopedPointer<QThread>    _thread;
    QScopedPointer<MeasurePAE> _measure;

};

#endif // MAINWINDOW_H
