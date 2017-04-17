

// Project
#include "settings.h"
#include "stagesettings.h"
#include "stagesettingsmodel.h"

// Qt
#include <QApplication>
#include <QScopedPointer>
#include <QTableView>
#include <QVector>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    // instantiate here
    //
    // MyWidget w;
    // w.show();
    //

    QVector<StageSettings> settings(4);
    QScopedPointer<StageSettingsModel> model(new StageSettingsModel);
    model->setSettings(settings);

    QTableView view;
    view.setModel(model.data());
    view.show();

    return app.exec();
}

