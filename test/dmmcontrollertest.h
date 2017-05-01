#ifndef DMMCONTROLLERTEST_H
#define DMMCONTROLLERTEST_H


// RsaToolbox
#include <TestClass.h>

// Qt
#include <QDir>
#include <QObject>


class DmmControllerTest : public RsaToolbox::TestClass
{
    Q_OBJECT
public:
    explicit DmmControllerTest(QObject *parent = 0);
    ~DmmControllerTest();

private slots:
    void test();

private:
    QDir _src;
    QDir _fixtures;
};

#endif // DMMCONTROLLERTEST_H
