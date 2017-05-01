#ifndef DMMTEST_H
#define DMMTEST_H


// RsaToolbox
#include <TestClass.h>

// Qt
#include <QDir>
#include <QObject>


class DmmTest : public RsaToolbox::TestClass
{
    Q_OBJECT

public:
    explicit DmmTest(QObject *parent = 0);
    ~DmmTest();

private slots:
    void test();

private:
    QDir _src;
    QDir _logDir;
    QDir _fixtures;
};

#endif // DMMTEST_H
