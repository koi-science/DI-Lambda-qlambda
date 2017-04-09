#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QWidget>
#include "usbhiddevicesmodel.h"
#include "dilambda.h"


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:
    void serial_ports_changed(QString port_name);


    void dilambda_connected(QString port_name);
    void dilambda_connection_failed(QString reason);

    void dilambda_disconnected();

    void dilambda_new_sample(QList<int> data);

    void connect_button_pressed();
    void disconnect_button_pressed();

    void take_blank1();
    void take_blank2();
    void take_blank3();

    void take_sample1();
    void take_sample2();
    void take_sample3();

    void update_labels();


private:
    void insert_sample(int i);

    Ui::MainWindow *ui;

    UsbHidDevicesModel devices_model;
    DiLambda *dilambda;

    float blanks[3];
    float samples[3];
    bool blank_needed[3];
};

#endif // MAINWINDOW_H
