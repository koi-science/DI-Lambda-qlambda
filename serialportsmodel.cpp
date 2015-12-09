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
//    qDebug() << "length = " << ports_list.length();
 //   qDebug() << "size = " << ports_list.size();

    return ports_list.length();
}

void SerialPortsModel::timerHit()
{
    int old_row_count = rowCount();

 //   this->beginResetModel();
    ports_list = QSerialPortInfo::availablePorts();


//    qDebug() << "Hello!";
//    qDebug() << ports_list.length();


    QModelIndex topLeft = createIndex(0,0);
    QModelIndex bottomRight = createIndex(qMax(ports_list.size(), old_row_count),0);
//    this->resetInternalData();
    emit dataChanged(topLeft, bottomRight);

//    this->endResetModel();

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

