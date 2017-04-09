#include "usbhiddevicesmodel.h"

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

    if (role == Qt::DisplayRole) {
        Q_ASSERT(index.row() < hid_devices.size());
        Q_ASSERT(index.column() == 0);

        auto it_dev = std::next(hid_devices.cbegin(), index.row());
        return QString("[%3:%4] %2, %1")
            .arg(QString::fromStdString(it_dev->manufacturer_string))
            .arg(QString::fromStdString(it_dev->product_string))
            .arg(it_dev->vid, 4, 16, QChar('0'))
            .arg(it_dev->pid, 4, 16, QChar('0'))
            ;
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
