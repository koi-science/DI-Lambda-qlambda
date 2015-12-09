#ifndef SERIALPORTSMODEL_H
#define SERIALPORTSMODEL_H

#include <QAbstractListModel>
#include <QSerialPortInfo>
#include <QSerialPort>
#include <QTimer>
#include <QDebug>

class SerialPortsModel : public QAbstractListModel
{
    Q_OBJECT
public:
    explicit SerialPortsModel(QObject *parent = 0);
    int rowCount(const QModelIndex &parent = QModelIndex()) const ;
    int columnCount(const QModelIndex &parent = QModelIndex()) const;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;

private:
    QTimer *timer;
    QList <QSerialPortInfo> ports_list;

signals:

public slots:
    void timerHit();
};

#endif // SERIALPORTSMODEL_H
