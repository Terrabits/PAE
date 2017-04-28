#ifndef MAINWINDOW_H
#define MAINWINDOW_H


// RsaToolbox
#include <Keys.h>
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

private slots:
    void run();

private:
    Ui::MainWindow *ui;

    RsaToolbox::Vna  *_vna;
    RsaToolbox::Keys *_keys;
    QVector<uint>     _channels;

    void init();
};

#endif // MAINWINDOW_H
