#ifndef DMMCONTROLLER_H
#define DMMCONTROLLER_H


// Project
#include "dmm.h"
#include "stagesettings.h"

// RsaToolbox
#include <Definitions.h>

// Qt
#include <QObject>
#include <QScopedPointer>
#include <QString>
#include <QVector>


class DmmController : public QObject
{
    Q_OBJECT
public:
    explicit DmmController(QObject *parent = 0);
    ~DmmController();

    void setStages(const QVector<StageSettings> &stages);

    bool isConnected() const;
    RsaToolbox::QMatrix2D readData();

signals:
    void error(const QString &message);

public slots:
    bool connect();
    bool setup(uint points);
    void start();

private:
    bool _isConnected;
    uint _points;
    QVector<StageSettings> _stages;
    QVector<QScopedPointer<Dmm>> _dmms;

    bool _setup(uint i);
    void clear();
};

#endif // DMMCONTROLLER_H
