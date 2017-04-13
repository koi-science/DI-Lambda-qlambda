#include "usbhiddevicesmodel.h"

#include <QDebug>

UsbHidDevicesModel::UsbHidDevicesModel(QObject *parent)
    : QAbstractListModel(parent)
{
    reenumerate();
}

int UsbHidDevicesModel::rowCount(const QModelIndex &parent) const
{
    // For list models only the root node (an invalid parent) should return the list's size. For all
    // other (valid) parents, rowCount() should return 0 so that it does not become a tree model.
    if (parent.isValid())
        return 0;

    return hid_devices.size();
}

QVariant UsbHidDevicesModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    Q_ASSERT(index.row() < hid_devices.size());
    Q_ASSERT(index.column() == 0);

    auto it_dev = std::next(hid_devices.cbegin(), index.row());

    if (role == Qt::DisplayRole) {
        return QString("[%3:%4] %2, %1")
            .arg(QString::fromStdString(it_dev->manufacturer_string))
            .arg(QString::fromStdString(it_dev->product_string))
            .arg(it_dev->vid, 4, 16, QChar('0'))
            .arg(it_dev->pid, 4, 16, QChar('0'))
            ;
    } else if (role == Qt::UserRole) {
        return QVariant::fromValue(*it_dev);
    }

    return QVariant();
}

void UsbHidDevicesModel::reenumerate()
{
    int old_row_count = hid_devices.size();
    hid_devices.clear();

    hid_device_info* enum_list = hid_enumerate(0, 0); // hid_enumerate(0x1b4f, 0x9204);
    // copy over to our memory, and free immediately
    for(auto* p = enum_list; p != nullptr; p = p->next) {
        hid_devices.append(HidDevice(p));
    }
    hid_free_enumeration(enum_list);

    emit dataChanged(createIndex(0, 0), createIndex(qMax(old_row_count, hid_devices.size()), 0));
}


UsbHidPollLoop::UsbHidPollLoop()
    : loop_thread(this)
    , current_device(nullptr)
{
    moveToThread(&loop_thread);
    QObject::connect(&loop_thread, SIGNAL(started()), this, SLOT(run()));
}

UsbHidPollLoop::~UsbHidPollLoop()
{
    loop_thread.quit();
    loop_thread.wait();
}

void UsbHidPollLoop::run()
{
    QByteArray buf(16, '\0');

    while (1) {
        if (!current_device) {
            loop_thread.msleep(1000);
        } else {
            int r = hid_read(current_device, (unsigned char*)buf.data(), buf.size());
            if (r == -1) {
                qDebug() << "hid read error: " << hid_error(current_device);
            } else if (r > 0) {
                buf.resize(r);
                emit dataReceived(buf);
            }
        }
    }
}

void UsbHidPollLoop::start()
{
    loop_thread.start();
}

void UsbHidPollLoop::disconnect()
{
    if (current_device) {
        hid_close(current_device);
        current_device = nullptr;
        emit disconnected();
    }
}

bool UsbHidPollLoop::connect(HidDevice dev) {
    disconnect();

    current_device = hid_open_path(dev.path.c_str());
    bool success = current_device != nullptr;
    if (success) {
        emit connected();
        loop_thread.requestInterruption();
    } else {
        qDebug() << "hid_open() failed!";
    }
    return success;
}
