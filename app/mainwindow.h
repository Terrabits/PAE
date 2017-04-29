#ifndef MAINWINDOW_H
#define MAINWINDOW_H


// Project
#include "traceswidget.h"

// RsaToolbox
#include <Keys.h>
#include <Shake.h>
#include <Vna.h>

// Qt
#include <QMainWindow>
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

public slots:
    bool close();

public slots:
    void showError(const QString &message);

private slots:
    void run();

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
};

#endif // MAINWINDOW_H
