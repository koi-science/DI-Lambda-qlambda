#ifndef DILAMBDA_H
#define DILAMBDA_H

#include <QObject>
#include <QSerialPort>
#include <QSerialPortInfo>

class DiLambda : public QObject
{
    Q_OBJECT
public:
    explicit DiLambda(QObject *parent = 0);

private:
    QSerialPort current_port;

signals:
    void new_sample(QList<int> data);
    void serial_connected(QString port_name);
    void serial_disconnected();
    void serial_connection_failure(QString reason);

public slots:
    void start_connect(QString port_name);
    void start_disconnect();

private slots:
    void data_available();
    void serial_port_error(QSerialPort::SerialPortError);
};

#endif // DILAMBDA_H
