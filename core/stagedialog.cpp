#include "stagedialog.h"
#include "ui_stagedialog.h"


// RsaToolbox
#include <Definitions.h>
using namespace RsaToolbox;

// Qt
#include <QApplication>
#include <QFileDialog>
#include <QFileInfo>
#include <QRegExp>
#include <QRegExpValidator>
#include <QScopedPointer>


StageDialog::StageDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::StageDialog)
{
    ui->setupUi(this);

    setWindowTitle("Stage settings");
    QRegExp regex("^[a-z_][0-9a-z_]*$", Qt::CaseInsensitive);
    QScopedPointer<QRegExpValidator> validator(new QRegExpValidator(regex));
    ui->name->setValidator(validator.take());
    ui->name->setFocus();
    ui->name->selectAll();

    ui->supplyVoltage->setParameterName("Supply voltage");
    ui->supplyVoltage->setUnitAbbrev("V");
    ui->supplyVoltage->setMinimum(1, SiPrefix::Femto);

    ui->shuntResistor->setParameterName("Shunt resistance");
    ui->shuntResistor->setUnitAbbrev("Ω");
    ui->shuntResistor->interpretMKeyAsMilli();
    ui->shuntResistor->setMinimum(1, SiPrefix::Femto);

    connect(ui->driverButton, SIGNAL(clicked()),
            this, SLOT(getDriver()));
}

StageDialog::~StageDialog()
{
    delete ui;
}

StageSettings StageDialog::settings() const {
    StageSettings _settings;
    _settings.name = ui->name->text();
    _settings.connectionType = ui->connectionType->currentType();
    _settings.address = ui->address->text();
    _settings.powerSupply_V = ui->supplyVoltage->value();
    _settings.shuntResistor_ohms = ui->shuntResistor->value();
    _settings.driverPath = _driver;
    return _settings;
}

void StageDialog::setSettings(const StageSettings &settings) {
    ui->name->setText(settings.name);
    ui->connectionType->setCurrentType(settings.connectionType);
    ui->address->setText(settings.address);
    ui->supplyVoltage->setValue(settings.powerSupply_V);
    ui->shuntResistor->setValue(settings.shuntResistor_ohms);
    setDriver(settings.driverPath);
}

void StageDialog::getDriver() {
    QString result = QFileDialog::getOpenFileName(QApplication::activeWindow(),
                                 "Open driver...",
                                 "",
                                 "Driver (*.json)");

    if (result.isEmpty())
        return;

    setDriver(result);
}
void StageDialog::setDriver(const QString &driver) {
    _driver = driver;
    ui->filename->setText(QFileInfo(driver).fileName());
}
