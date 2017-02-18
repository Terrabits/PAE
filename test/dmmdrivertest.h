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
    void open();
    void save();

private:
    QDir _src;
};

#endif // DMMDRIVERTEST_H
