#ifndef PULSEGENWIDGET_H
#define PULSEGENWIDGET_H

#include <QWidget>

namespace Ui {
class PulseGenWidget;
}

class PulseGenWidget : public QWidget
{
    Q_OBJECT

public:
    explicit PulseGenWidget(QWidget *parent = 0);
    ~PulseGenWidget();

private:
    Ui::PulseGenWidget *ui;
};

#endif // PULSEGENWIDGET_H
