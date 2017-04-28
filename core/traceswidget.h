#ifndef TRACESWIDGET_H
#define TRACESWIDGET_H


// RsaToolbox
#include <Keys.h>
#include <Vna.h>

// Qt
#include <QVector>
#include <QWidget>


namespace Ui {
class TracesWidget;
}

class TracesWidget : public QWidget
{
    Q_OBJECT

public:
    explicit TracesWidget(QWidget *parent = 0);
    ~TracesWidget();

    void setVna (RsaToolbox::Vna  *vna );
    void setKeys(RsaToolbox::Keys *keys);

private slots:
    void on_channel_currentIndexChanged(int index);

private:
    Ui::TracesWidget *ui;

    RsaToolbox::Vna *_vna;
    RsaToolbox::Keys *_keys;

    QVector<uint> _channels;
};

#endif // TRACESWIDGET_H
