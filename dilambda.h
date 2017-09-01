#ifndef DILAMBDA_H
#define DILAMBDA_H

#include <QObject>
#include <QSerialPort>
#include <QSerialPortInfo>
#include "usbhiddevicesmodel.h"

class DiLambda : public QObject
{
    Q_OBJECT
public:
    explicit DiLambda(QObject *parent = 0);

private:
    QSerialPort current_port;
    UsbHidPollLoop usbhid_loop;

signals:
    void new_sample(QList<int> data);
    void serial_connected(QString port_name);
    void serial_disconnected();
    void serial_connection_failure(QString reason);

public slots:
    void start_connect(QString port_name);
    void start_disconnect();

    void start_connect_hid(HidDevice);

private slots:
    void data_available();
    void serial_port_error(QSerialPort::SerialPortError);

    void hid_data_recv(const QByteArray&);
};

#endif // DILAMBDA_H
