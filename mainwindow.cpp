#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QMessageBox>
#include <QTime>
#include <QDebug>
#include <math.h>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , devices_model()
{
    ui->setupUi(this);
    ui->pushButton_disconnect->hide();

    //ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
//    serial_ports_model = new SerialPortsModel();

    ui->comboBox->setModel(&devices_model);

    dilambda = new DiLambda();

    connect(ui->comboBox, SIGNAL(activated(QString)), this, SLOT(serial_ports_changed(QString)));
    connect(ui->pushButton_connect, SIGNAL(clicked()), this, SLOT(connect_button_pressed()));
    connect(ui->pushButton_disconnect, SIGNAL(clicked()), this, SLOT(disconnect_button_pressed()));

    connect(ui->pushButton_blank1, SIGNAL(clicked()), this, SLOT(take_blank1()));
    connect(ui->pushButton_blank2, SIGNAL(clicked()), this, SLOT(take_blank2()));
    connect(ui->pushButton_blank3, SIGNAL(clicked()), this, SLOT(take_blank3()));

    connect(ui->pushButton_sample1, SIGNAL(clicked()), this, SLOT(take_sample1()));
    connect(ui->pushButton_sample2, SIGNAL(clicked()), this, SLOT(take_sample2()));
    connect(ui->pushButton_sample3, SIGNAL(clicked()), this, SLOT(take_sample3()));


    connect(dilambda, SIGNAL(serial_connected(QString)), this, SLOT(dilambda_connected(QString)));
    connect(dilambda, SIGNAL(serial_connection_failure(QString)), this, SLOT(dilambda_connection_failed(QString)));
    connect(dilambda, SIGNAL(serial_disconnected()), this, SLOT(dilambda_disconnected()));

    connect(dilambda, SIGNAL(new_sample(QList<int>)), this, SLOT(dilambda_new_sample(QList<int>)));

    connect(ui->radioButton_abs1, SIGNAL(clicked()), this, SLOT(update_labels()));
    connect(ui->radioButton_abs2, SIGNAL(clicked()), this, SLOT(update_labels()));
    connect(ui->radioButton_abs3, SIGNAL(clicked()), this, SLOT(update_labels()));

    connect(ui->radioButton_t1, SIGNAL(clicked()), this, SLOT(update_labels()));
    connect(ui->radioButton_t2, SIGNAL(clicked()), this, SLOT(update_labels()));
    connect(ui->radioButton_t3, SIGNAL(clicked()), this, SLOT(update_labels()));


    ui->statusBar->showMessage("Disconnected");

    for (int i = 0; i < 3; i++) {
        blank_needed[i] = false;
        blanks[i] = 0;
    }
}

void MainWindow::connect_button_pressed()
{
    //QString port_name = ui->comboBox->currentText();
    HidDevice hid_dev = ui->comboBox->currentData(Qt::UserRole).value<HidDevice>();

    ui->comboBox->setEnabled(false);
    //    ui->pushButton_connect->setEnabled(false);
    ui->statusBar->showMessage("Connecting...");
    dilambda->start_connect_hid(hid_dev);
}

void MainWindow::disconnect_button_pressed()
{
    ui->statusBar->showMessage("Disconnecting...");
    dilambda->start_disconnect();
}

void MainWindow::serial_ports_changed(QString port_name)
{
    qDebug() << "serial_ports_changed" << port_name;

    if (port_name != "") {
        ui->pushButton_connect->setEnabled(true);
    } else {
        ui->pushButton_connect->setEnabled(false);
    }
}

void MainWindow::dilambda_connected(QString port_name)
{
    ui->groupBox_ch1->setEnabled(true);
    ui->groupBox_ch2->setEnabled(true);
    ui->groupBox_ch3->setEnabled(true);

    ui->pushButton_connect->hide();
    ui->pushButton_disconnect->show();

    ui->statusBar->showMessage("Connected to " + port_name);

}

void MainWindow::dilambda_connection_failed(QString reason)
{
    ui->statusBar->showMessage("Disconnected");
    ui->comboBox->setEnabled(true);

    ui->pushButton_disconnect->hide();
    ui->pushButton_connect->show();

    QMessageBox::critical(0, "Serial connection failure", "Serial connection failure: " +  reason);

}

void MainWindow::dilambda_disconnected()
{
    ui->groupBox_ch1->setEnabled(false);
    ui->groupBox_ch2->setEnabled(false);
    ui->groupBox_ch3->setEnabled(false);
    ui->statusBar->showMessage("Disconnected");
    ui->comboBox->setEnabled(true);

    ui->pushButton_disconnect->hide();
    ui->pushButton_connect->show();
}

void MainWindow::update_labels()
{
    if(blanks[0] != 0) {
        if (ui->radioButton_abs1->isChecked()) {
            ui->label_1->setText(QString::number(-log10((samples[0] / blanks[0])), 'f', 2));
        } else if (ui->radioButton_t1->isChecked()) {
            ui->label_1->setText(QString::number((double) (100 * samples[0] / blanks[0]), 'f', 1) + "%");

        } else  {
            ui->label_1->setText("?.??");
        }
    }

    if(blanks[1] != 0) {
        if (ui->radioButton_abs2->isChecked()) {
            ui->label_2->setText(QString::number(-log10(samples[1] / blanks[1]), 'f', 2));
        } else if (ui->radioButton_t2->isChecked()) {
            ui->label_2->setText(QString::number(100 * samples[1] / blanks[1], 'f', 1) + "%");

        } else  {
            ui->label_2->setText("?.??");
        }
    }

    if(blanks[2] != 0) {
        if (ui->radioButton_abs3->isChecked()) {
            ui->label_3->setText(QString::number(-log10( samples[2] / blanks[2]), 'f', 2));
        } else if (ui->radioButton_t3->isChecked()) {
            ui->label_3->setText(QString::number(100 * samples[2] / blanks[2], 'f', 1) + "%");

        } else  {
            ui->label_3->setText("?.??");
        }
    }

}

void MainWindow::dilambda_new_sample(QList<int> data)
{
    qDebug() << "dilambda_new_sample" << data[0] << data[1] << data[2];
    for (int i = 0; i < 3; i++) {
        if (blank_needed[i]) {
            blanks[i] = data[i];
            blank_needed[i] = false;
        }
        samples[i] = data[i];
    }

    update_labels();
}

void MainWindow::take_blank1()
{
    blank_needed[0] = true;
}

void MainWindow::take_blank2()
{
    blank_needed[1] = true;
}

void MainWindow::take_blank3()
{
    blank_needed[2] = true;
}

void MainWindow::insert_sample(int i)
{
    qDebug() << "take_sample1";
    int row = ui->tableWidget->rowCount();

    ui->tableWidget->insertRow(row);
    ui->tableWidget->setItem(row, 0, new QTableWidgetItem(QString::number(ui->tableWidget->rowCount())));
    ui->tableWidget->setItem(row, 1, new QTableWidgetItem(QString::number(i + 1)));
    ui->tableWidget->setItem(row, 2, new QTableWidgetItem(QString::number(-log10(samples[i] / blanks[i]), 'f', 2)));
    ui->tableWidget->setItem(row, 3, new QTableWidgetItem(QString::number(100 * samples[i] / blanks[i], 'f', 1) + " %"));

    QTime timestamp = QTime::currentTime();

    ui->tableWidget->setItem(row, 4, new QTableWidgetItem(timestamp.toString()));
}

void MainWindow::take_sample1()
{
    insert_sample(0);
}

void MainWindow::take_sample2()
{
    insert_sample(1);
}

void MainWindow::take_sample3()
{
    insert_sample(2);
}

MainWindow::~MainWindow()
{
    delete ui;
}
