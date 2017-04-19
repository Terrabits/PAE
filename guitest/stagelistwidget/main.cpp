

// Project
#include "settings.h"
#include "stagelistwidget.h"

// Qt
#include <QApplication>
#include <QScopedPointer>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    StageListWidget widget;
    widget.show();
    return app.exec();
}

