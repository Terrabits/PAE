#ifndef DMMDRIVERTEST_H
#define DMMDRIVERTEST_H

// RsaToolbox
#include <TestClass.h>

// Qt
#include <QDir>
#include <QObject>

class DmmDriverTest : public RsaToolbox::TestClass
{
    Q_OBJECT
public:
    explicit DmmDriverTest(QObject *parent = 0);

private slots:
    // Once
    virtual void initTestCase();
    virtual void cleanupTestCase();

    void openTest();
    void saveTest();

    void isValidTest_data();
    void isValidTest();

private:
    QDir _src;
    QDir _fixtures;
    QDir _dmmdrivertestFixtures;
    QDir _temp;
};

#endif // DMMDRIVERTEST_H
