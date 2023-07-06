#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtCore>
#include <QDebug>
#include <QList>
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
    void    slotGetPID();
    void    on_OpenSerialButton_clicked();

    void on_btnGetTemp_clicked();

    void on_btnSetCom_clicked();

    void on_btnCtrlTemp_clicked();

    void on_btnSetTemp_clicked();

    void on_btnSTBY_clicked();

    void on_btnAT_clicked();

    void on_btnGetPID_clicked();

private:
    QByteArray      messageSend;
    QByteArray      messageRecv;
    QString         csvFile;

    Ui::MainWindow  *ui;
    QSerialPort     *serial;
    QTimer          *timer;
    QTimer          *PIDtimer;
};
#endif // MAINWINDOW_H
