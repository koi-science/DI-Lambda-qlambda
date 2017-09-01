#ifndef USBHIDDEVICESMODEL_H
#define USBHIDDEVICESMODEL_H

#include <QAbstractListModel>
#include <QThread>

#include <hidapi/hidapi.h> /* signal11/hidapi */

struct HidDevice {
    std::string path;
    std::string manufacturer_string;
    std::string product_string;
    uint16_t vid, pid;

    HidDevice() = default;
    HidDevice(const HidDevice&) = default;
    ~HidDevice() = default;

    HidDevice(hid_device_info* hidapi_devinfo) {
        this->path = QString::fromUtf8(hidapi_devinfo->path).toStdString();
        this->manufacturer_string = QString::fromWCharArray(hidapi_devinfo->manufacturer_string).toStdString();
        this->product_string = QString::fromWCharArray(hidapi_devinfo->product_string).toStdString();

        this->vid = hidapi_devinfo->vendor_id;
        this->pid = hidapi_devinfo->product_id;
    }
};
Q_DECLARE_METATYPE(HidDevice)


class UsbHidDevicesModel : public QAbstractListModel
{
    Q_OBJECT

public:
    explicit UsbHidDevicesModel(QObject *parent = 0);

    // Basic functionality:
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

private:
    QList<HidDevice> hid_devices;

    void reenumerate();
};


class UsbHidPollLoop : public QObject {
    Q_OBJECT

    QThread loop_thread;
    hid_device* current_device;

public:
    UsbHidPollLoop();
    ~UsbHidPollLoop();

    void start();
    bool connect(HidDevice);
    void disconnect();

public slots:
    void run();

signals:
    void dataReceived(const QByteArray&);
    void disconnected();
    void connected();
};

#endif // USBHIDDEVICESMODEL_H
