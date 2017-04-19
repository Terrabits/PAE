

// Project
#include "settings.h"
#include "stagedialog.h"

// Qt
#include <QApplication>
#include <QScopedPointer>
#include <QTableView>
#include <QVector>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    StageDialog dialog;
    dialog.show();
    return app.exec();
}

