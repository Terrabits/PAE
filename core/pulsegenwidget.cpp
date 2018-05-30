#include "pulsegenwidget.h"
#include "ui_pulsegenwidget.h"

PulseGenWidget::PulseGenWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PulseGenWidget)
{
    ui->setupUi(this);
}

PulseGenWidget::~PulseGenWidget()
{
    delete ui;
}
