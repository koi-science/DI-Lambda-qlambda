#include <QWidget>
#include <QDebug>
#include "dilambda.h"

#include <QList>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonDocument>

DiLambda::DiLambda(QObject *parent) :
    QObject(parent)
{
    connect(&current_port, SIGNAL(readyRead()), this, SLOT(data_available()));
    connect(&current_port, SIGNAL(error(QSerialPort::SerialPortError)), this, SLOT(serial_port_error(QSerialPort::SerialPortError)));

    connect(&usbhid_loop, SIGNAL(dataReceived(const QByteArray&)), this, SLOT(hid_data_recv(const QByteArray&)));
    usbhid_loop.start();
}

void DiLambda::start_connect(QString port_name)
{
    qDebug() << "start_connect " << port_name;

    if (current_port.isOpen())
        current_port.close();

    current_port.setBaudRate(QSerialPort::Baud9600);
    current_port.setDataBits(QSerialPort::Data8);
    current_port.setParity(QSerialPort::NoParity);
    current_port.setStopBits(QSerialPort::OneStop);
    current_port.setPortName(port_name);

    if (current_port.open(QIODevice::ReadWrite)) {
        emit serial_connected(port_name);

        qDebug() << "Serial port error " << current_port.errorString() << ":" << current_port.error();
    }
}

void DiLambda::start_disconnect()
{
    current_port.close();
    emit serial_disconnected();
}

void DiLambda::data_available()
{
    while (current_port.canReadLine()) {
        QByteArray line = current_port.readLine();
        qDebug() << "read:" + line;

        if (line.startsWith("J: ")) {
            qDebug("looks like json");
            QJsonDocument json = QJsonDocument::fromJson(line.mid(3)); // Skip the first 3 characters

            qDebug() << "Jason: " << json.toJson();

            QJsonObject jo = json.object();
            QJsonArray bg = jo["bg"].toArray();
            QJsonArray sample = jo["signal"].toArray();

            QList<int> data_list;

            for (int i = 0; i < 3; i++) {
                data_list.append(sample[i].toInt() - bg[i].toInt());
            }

            emit new_sample(data_list);
        }
    }
}

void DiLambda::serial_port_error(QSerialPort::SerialPortError err)
{
    if (err != QSerialPort::NoError) {
        qDebug() << "Serial port error " + current_port.errorString() + ":" + current_port.error();
        emit serial_connection_failure(current_port.errorString());
    }
}


void DiLambda::hid_data_recv(const QByteArray& data)
{
    qDebug() << "Usb HID data: " << data;
}

void DiLambda::start_connect_hid(HidDevice dev)
{
    usbhid_loop.connect(dev);
}
