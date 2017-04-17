#ifndef STAGESETTINGSMODEL_H
#define STAGESETTINGSMODEL_H


// Project
#include "stagesettings.h"

// Qt
#include <QAbstractTableModel>
#include <QVector>


class StageSettingsModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    StageSettingsModel(QObject *parent = 0);
    ~StageSettingsModel();

    enum /**/ Column {
        name = 0,
        powerSupply,
        shuntResistor,
        connectionType,
        address,
        driver
    };
    static const int COLUMNS;

    virtual int columnCount(const QModelIndex &parent) const;
    virtual int rowCount(const QModelIndex &parent) const;
    virtual QVariant data(const QModelIndex &index, int role) const;
    virtual bool setData(const QModelIndex &index, const QVariant &value, int role);
    virtual QModelIndex index(int row, int column, const QModelIndex &parent) const;
    virtual QModelIndex parent(const QModelIndex &child) const;

    QVector<StageSettings> settings() const;
    void setSettings(const QVector<StageSettings> &settings);

private:
    QVector<StageSettings> _settings;

    bool isValidIndex(const QModelIndex &index) const;
    QVariant displayData(const QModelIndex &index) const;
    QVariant editData(const QModelIndex &index) const;
};

#endif // STAGESETTINGSMODEL_H
