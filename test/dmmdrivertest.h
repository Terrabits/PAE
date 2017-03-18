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

    void open();
    void save();

private:
    QDir _src;
    QDir _fixtures;
    QDir _temp;
};

#endif // DMMDRIVERTEST_H
