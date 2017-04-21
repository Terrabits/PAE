

// Project
#include "settings.h"
#include "stagedialog.h"
#include "stagesettings.h"

// Qt
#include <QApplication>
#include <QDebug>
#include <QScopedPointer>
#include <QTableView>
#include <QVector>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    StageDialog dialog;
    dialog.show();
    app.exec();

    qDebug() << "Dialog accepted? " << (dialog.result() == QDialog::Accepted);
    if (dialog.result() == QDialog::Accepted) {
        StageSettings s = dialog.settings();
        qDebug() << "name:    " << s.name;
        qDebug() << "Power:   " << s.powerSupply_V;
        qDebug() << "Shunt:   " << s.shuntResistor_ohms;
        qDebug() << "Connect: " << s.connectionTypeString();
        qDebug() << "Addr:    " << s.address;
        qDebug() << "Driver:  " << s.driverPath;
    }
}

