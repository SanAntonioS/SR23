#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtCore>
#include <QDebug>
#include <QMessageBox>
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include <QtCharts>
using namespace QtCharts;

#include "appdata.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void    creatCharts();
    void    SerialPortInit();
    void    RefreshSerialPort(int index);
    void    processData(QString data);


private slots:
    void    LED(bool changeColor);
    void    DataReceived();
    void    DataSend();
    void    slotGetTemp();
    void    on_OpenSerialButton_clicked();

    void on_btnGetTemp_clicked();

private:
    QByteArray      messageSend;
    QByteArray      messageRecv;
    QString         csvFile;

    Ui::MainWindow  *ui;
    QSerialPort     *serial;
    QTimer          *timer;
};
#endif // MAINWINDOW_H
