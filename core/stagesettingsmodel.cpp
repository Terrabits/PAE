#include "stagesettingsmodel.h"


// RsaToolbox
#include <General.h>
using namespace RsaToolbox;

// Qt
#include <QDebug>


StageSettingsModel::StageSettingsModel(QObject *parent) :
    QAbstractTableModel(parent)
{

}

StageSettingsModel::~StageSettingsModel()
{

}

const int StageSettingsModel::COLUMNS = 6;
int StageSettingsModel::columnCount(const QModelIndex &parent) const {
    if (parent.isValid())
        return -1;

    return COLUMNS;
}

int StageSettingsModel::rowCount(const QModelIndex &parent) const {
    if (parent.isValid())
        return -1;

    return _settings.size();
}
QVariant StageSettingsModel::headerData(int section, Qt::Orientation orientation, int role) const {
    if (orientation == Qt::Vertical || role != Qt::DisplayRole) {
        return QAbstractTableModel::headerData(section, orientation, role);
    }

    switch(section) {
    case Column::name:
        return "Name";
    case Column::powerSupply:
        return "P. Supply";
    case Column::shuntResistor:
        return "Shunt R";
    case Column::connectionType:
        return "Connect";
    case Column::address:
        return "Address";
    case Column::driver:
        return "Driver";
    default:
        return QVariant();
    }
}
QVariant StageSettingsModel::data(const QModelIndex &index, int role) const {
    if (!isValidIndex(index))
        return QVariant();

    switch (role) {
    case Qt::DisplayRole:
        return displayData(index);
    case Qt::EditRole:
        return editData(index);
    default:
        return QVariant();
    }
}
bool StageSettingsModel::setData(const QModelIndex &index, const QVariant &value, int role) {
    if (!isValidIndex(index))
        return false;
    if (role != Qt::EditRole)
        return false;

    const int row = index.row();
    switch(index.column()) {
    case Column::name:
        _settings[row].name = value.toString();
        emit dataChanged(index, index);
        return true;
    case Column::powerSupply:
        _settings[row].powerSupply_V = value.toDouble();
        emit dataChanged(index, index);
        return true;
    case Column::shuntResistor:
        _settings[row].shuntResistor_ohms = value.toDouble();
        emit dataChanged(index, index);
        return true;
    case Column::connectionType:
        _settings[row].connectionType = value.value<RsaToolbox::ConnectionType>();
        emit dataChanged(index, index);
        return true;
    case Column::address:
        _settings[row].address = value.toString();
        emit dataChanged(index, index);
        return true;
    case Column::driver:
        _settings[row].driverPath = value.toString();
        emit dataChanged(index, index);
        return true;
    default:
        return false;
    }
}
QModelIndex StageSettingsModel::parent(const QModelIndex &child) const {
    Q_UNUSED(child);
    return QModelIndex();
}

bool StageSettingsModel::insertRows(int row, int count, const QModelIndex &parent) {
    if (parent.isValid())
        return false;

    beginInsertRows(parent, row, row + count - 1);
    _settings.insert(row, count, StageSettings());
    endInsertRows();
    return true;
}
bool StageSettingsModel::removeRows(int row, int count, const QModelIndex &parent) {
    if (parent.isValid())
        return false;

    beginRemoveRows(parent, row, row + count - 1);
    _settings.remove(row, count);
    endRemoveRows();
    return true;
}
bool StageSettingsModel::moveRows(const QModelIndex &sourceParent, int sourceRow, int count, const QModelIndex &destinationParent, int destinationChild) {
    qDebug() << "Move " << sourceRow << "-" << count << " => " << destinationChild;
    if (sourceParent.isValid())
        return false;
    if (destinationParent.isValid())
        return false;

//    beginMoveRows(sourceParent, sourceRow, sourceRow + count - 1, destinationParent, destinationChild);
    beginResetModel();
    qDebug() << "  Begin moving...";
    qDebug() << "  _settings.size: " << _settings.size();
    QVector<StageSettings> source = _settings.mid(sourceRow, count);
    qDebug() << "  source.size: " << source.size();
    _settings.remove(sourceRow, count);
    qDebug() << "  _settings.size: " << _settings.size();
    while (!source.isEmpty()) {
        qDebug() << "  Inserting...";
        _settings.insert(destinationChild, source.takeLast());
    }
    qDebug() << "  source.size: " << source.size();
    qDebug() << "  _settings.size: " << _settings.size();
//    endMoveRows();
    endResetModel();
    qDebug() << "End";
    return true;
}

QVector<StageSettings> StageSettingsModel::settings() const {
    return _settings;
}
void StageSettingsModel::setSettings(const QVector<StageSettings> &settings) {
    beginResetModel();
    _settings = settings;
    endResetModel();
}

bool StageSettingsModel::isValidIndex(const QModelIndex &index) const {
    if (!index.isValid())
        return false;
    if (index.parent().isValid())
        return false;
    if (index.row() < 0)
        return false;
    if (index.row() >= _settings.size())
        return false;
    if (index.column() < 0)
        return false;
    if (index.column() >= COLUMNS)
        return false;

    return true;
}
QVariant StageSettingsModel::displayData(const QModelIndex &index) const {
    StageSettings s = _settings[index.row()];

    switch(index.column()) {
    case Column::name:
        return s.name;
    case Column::powerSupply:
        return formatValue(s.powerSupply_V, 3, "V");
    case Column::shuntResistor:
        return formatValue(s.shuntResistor_ohms, 3, "Ω");
    case Column::connectionType:
        return s.connectionTypeString();
    case Column::address:
        return s.address;
    case Column::driver:
        return s.driverFilename();
    default:
        return QVariant();
    }
}
QVariant StageSettingsModel::editData(const QModelIndex &index) const {
    StageSettings s = _settings[index.row()];

    switch(index.column()) {
    case Column::name:
        return s.name;
    case Column::powerSupply:
        return s.powerSupply_V;
    case Column::shuntResistor:
        return s.shuntResistor_ohms;
    case Column::connectionType:
        return s.connectionType;
    case Column::address:
        return s.address;
    case Column::driver:
        return s.driverPath;
    default:
        return QVariant();
    }
}
