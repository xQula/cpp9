#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QMessageBox>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    udpWorker = new UDPworker(this);
    udpWorker->InitSocket();

    connect(udpWorker, qOverload<QDateTime>(&UDPworker::sig_sendTimeToGUI), this, qOverload<QDateTime>(&MainWindow::DisplayTime));
    connect(udpWorker, qOverload<qsizetype, QString>(&UDPworker::sig_sendSizeByteToGUI), this, qOverload<qsizetype, QString>(&MainWindow::DisplayTime));

    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, [&]{

        QDateTime dateTime = QDateTime::currentDateTime();

        QByteArray dataToSend;
        QDataStream outStr(&dataToSend, QIODevice::WriteOnly);

        outStr << dateTime;

        udpWorker->SendDatagram(dataToSend, static_cast<int>(SignalOption::One));
        timer->start(TIMER_DELAY);

    });
    connect(this, &MainWindow::sg_in_text_send, this, [this]{
        QString str = ui->te_inText->toPlainText();
        if(!str.size()){
            QMessageBox *mes_box = new QMessageBox(this);
            mes_box->setText("Окно ввода данных пусто!");
            mes_box->setIcon(QMessageBox::Warning);
            mes_box->show();
        }else{
            QByteArray dataToSend;
            QDataStream outStr(&dataToSend, QIODevice::WriteOnly);
            outStr << str;
            udpWorker->SendDatagram(dataToSend,static_cast<int>(SignalOption::Two));
        }
    });


}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_pb_start_clicked()
{
    timer->start(TIMER_DELAY);
}


void MainWindow::DisplayTime(QDateTime data)
{
    counterPck++;
    if(counterPck % 20 == 0){
        ui->te_result->clear();
    }

    ui->te_result->append("Текущее время: " + data.toString() + ". "
                "Принято пакетов " + QString::number(counterPck));


}


void MainWindow::on_pb_stop_clicked()
{
    timer->stop();
}

void MainWindow::DisplayTime(qsizetype size, const QString str_send)
{
    ui->te_result->append("Принято сообщение от "+ str_send +", размер сообщения " + QString::number(size));
}


void MainWindow::on_pb_sendText_clicked()
{
    sg_in_text_send();
}

