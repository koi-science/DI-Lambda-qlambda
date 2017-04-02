#include "serialportsmodel.h"

SerialPortsModel::SerialPortsModel(QObject *parent) :
    QAbstractListModel(parent)
{
    timer = new QTimer(this);
    timer->setInterval(1000);
    connect(timer, SIGNAL(timeout()) , this, SLOT(timerHit()));
    timerHit();
    timer->start();
}

int SerialPortsModel::columnCount(const QModelIndex & /*parent */) const
{
    return 1;
}

int SerialPortsModel::rowCount(const QModelIndex & /*parent */) const
{
    return ports_list.length();
}

void SerialPortsModel::timerHit()
{
    int old_row_count = rowCount();

    ports_list = QSerialPortInfo::availablePorts();

    QModelIndex topLeft = createIndex(0,0);
    QModelIndex bottomRight = createIndex(qMax(ports_list.size(), old_row_count),0);

    emit dataChanged(topLeft, bottomRight);
}

QVariant SerialPortsModel::data(const QModelIndex &index, int role) const
{
    int row = index.row();
    int col = index.column();

//    qDebug() << "data" << row << " " << col;
    if (role == Qt::DisplayRole)
    {
        if (col == 0 and row < ports_list.length()) {
                return ports_list.at(row).portName();
        }
    }
    return QVariant();
}

