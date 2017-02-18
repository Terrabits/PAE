#ifndef KEYSIGHTDMMTEST_H
#define KEYSIGHTDMMTEST_H

// Project
#include "keysightdmm.h"

// RsaToolbox
#include <GenericBus.h>
#include <TestClass.h>
#include <Vna.h>

// Qt
#include <QDebug>
#include <QObject>
#include <QScopedPointer>
#include <QString>
#include <QTest>

class KeysightDmmTest : public RsaToolbox::TestClass
{
    Q_OBJECT

public:
    KeysightDmmTest(RsaToolbox::ConnectionType vnaType, const QString &vnaAddress, RsaToolbox::ConnectionType dmmType, const QString &dmmAddress, QObject *parent = 0);
    ~KeysightDmmTest();

private slots:
    // Once
    virtual void initTestCase();
    virtual void cleanupTestCase();

    // Per test
    virtual void init();
    virtual void cleanup();

    // Tests
    void idString();
    void settings();
    void measure();

private:
    RsaToolbox::ConnectionType      _vnaType;
    QString                         _vnaAddress;
    QScopedPointer<RsaToolbox::Vna> _vna;

    RsaToolbox::ConnectionType      _dmmType;
    QString                         _dmmAddress;
    QScopedPointer<KeysightDmm>     _dmm;
};

#endif // KEYSIGHTDMMTEST_H
