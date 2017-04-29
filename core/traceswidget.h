#ifndef TRACESWIDGET_H
#define TRACESWIDGET_H


// RsaToolbox
#include <Keys.h>
#include <Vna.h>

// Qt
#include <QComboBox>
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

    enum /*class*/ Calculation {
        powerAddedEfficiency = 0,
        drainEfficiency      = 1
    };

    void loadKeys();
    void saveKeys() const;

    void setAccess(RsaToolbox::Vna *vna, RsaToolbox::Keys *keys);

    bool hasAcceptableInput(Calculation calculation, QString &message);

    bool isInputTrace  () const;
    bool isGainTrace   () const;
    bool isOutputTrace () const;
    QString inputTrace () const;
    QString gainTrace  () const;
    QString outputTrace() const;

signals:
    void error(const QString &message);

private slots:
    void on_clearInputPower_clicked();
    void on_clearGain_clicked();
    void on_clearOutputPower_clicked();

private:
    Ui::TracesWidget *ui;

    bool isVna () const;
    mutable RsaToolbox::Vna *_vna;

    bool isKeys() const;
    mutable RsaToolbox::Keys *_keys;

    void clearInputTrace ();
    void clearGainTrace  ();
    void clearOutputTrace();
    void clearTraces     ();
    void updateTraces    ();
    void setInputTrace (const QString &name);
    void setGainTrace  (const QString &name);
    void setOutputTrace(const QString &name);
    void setCombo(QComboBox *combo, const QString &name);
};

#endif // TRACESWIDGET_H
