#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(slotGetTemp()));

    creatCharts();
    SerialPortInit();
}

MainWindow::~MainWindow()
{
    delete ui;
}

//创建表
void MainWindow::creatCharts(){
    QChart *qchart = new QChart;

    //把chart放到容器里
    ui->graphicsView->setChart(qchart);
    //设置抗锯齿
    ui->graphicsView->setRenderHint(QPainter::Antialiasing);

    //创建两条线
    QLineSeries *ser0 = new QLineSeries;
    QLineSeries *ser1 = new QLineSeries;
    //设置名字
    ser0->setName("温度");
    ser1->setName("功率");
    //放入charts里
    qchart->addSeries(ser0);
    qchart->addSeries(ser1);

    //创建x坐标
    QValueAxis *QaX = new QValueAxis;
    //设置范围
    QaX->setRange(0,10);
    QaX->setTickCount(10 + 1);

    //创建y坐标
    QValueAxis *QaY = new QValueAxis;
    //设置范围
    QaY->setRange(0,100);
    QaY->setTickCount(10 + 1);

    //将线条放入表中
    qchart->setAxisX(QaX,ser0);
    qchart->setAxisY(QaY,ser0);
    qchart->setAxisX(QaX,ser1);
    qchart->setAxisY(QaY,ser1);
}

void MainWindow::SerialPortInit()
{
    serial = new QSerialPort;                       //申请内存,并设置父对象

    // 获取计算机中有效的端口号，然后将端口号的名称给端口选择控件
    foreach(const QSerialPortInfo &info,QSerialPortInfo::availablePorts())
    {
        serial->setPort(info);                      // 在对象中设置串口
        if(serial->open(QIODevice::ReadWrite))      // 以读写方式打开串口
        {
            ui->PortBox->addItem(info.portName());  // 添加计算机中的端口
            serial->close();                        // 关闭
        } else
        {
            qDebug() << "串口打开失败，请重试";
        }
    }

    // 参数配置
    serial->setBaudRate(QSerialPort::Baud9600);
    serial->setParity(QSerialPort::NoParity);
    serial->setDataBits(QSerialPort::Data8);
    serial->setStopBits(QSerialPort::OneStop);
    serial->setFlowControl(QSerialPort::NoFlowControl);

    // 刷新串口
    RefreshSerialPort(0);

    connect(serial,&QSerialPort::readyRead,this,&MainWindow::DataReceived);      // 接收数据
//    connect(ui->SendStopOrder,&QPushButton::clicked,this,&MainWindow::DataSend);
//    connect(ui->SendButton,&QPushButton::clicked,this,&MainWindow::DataSend);    // 发送数据
}

void MainWindow::RefreshSerialPort(int index)
{
    QStringList portNameList;                                        // 存储所有串口名
    if(index != 0)
    {
        serial->setPortName(ui->PortBox->currentText());             //设置串口号
    }
    else
    {
        ui->PortBox->clear();                                        //关闭串口号
        ui->PortBox->addItem("刷新");                                //添加刷新
        foreach(const QSerialPortInfo &info,QSerialPortInfo::availablePorts()) //添加新串口
        {
            portNameList.append(info.portName());
        }
        ui->PortBox->addItems(portNameList);
        ui->PortBox->setCurrentIndex(1);                             // 当前串口号为COM1
        serial->setPortName(ui->PortBox->currentText());             //设置串口号
   }
}

void  MainWindow::LED(bool changeColor)
{
    if(changeColor == false)
        ui->LED->setStyleSheet("background-color: qradialgradient(spread:pad, cx:0.5, cy:0.5, radius:0.5, fx:0.5, fy:0.5, stop:0 rgba(0, 229, 0, 255), stop:1 rgba(255, 255, 255, 255));border-radius:12px;");
    else
        ui->LED->setStyleSheet("background-color: qradialgradient(spread:pad, cx:0.5, cy:0.5, radius:0.5, fx:0.5, fy:0.5, stop:0 rgba(255, 0, 0, 255), stop:1 rgba(255, 255, 255, 255));border-radius:12px;");
}

void MainWindow::processData(QString data)
{
    messageSend.clear();
    uchar beginBuf[4] = {0x02, 0x30, 0x31, 0x31};
    uchar endBuf[3] = {0x03, 0x0d, 0x0a};
    for (uint i=0;i < sizeof (beginBuf);i++) {
        messageSend.append(beginBuf[i]);
    }
    messageSend.append(data);
    for (uint i=0;i < sizeof (endBuf);i++) {
        messageSend.append(endBuf[i]);
    }
}


void MainWindow::DataReceived()
{
    messageRecv.append(serial->readAll());
}

void MainWindow::DataSend()
{
    qDebug()<<"DataSend:"<<messageSend;
    serial->write(messageSend);
}

void MainWindow::slotGetTemp()
{
    static int count = 0;
    static float Temperature, Power;

    if(count%2 == 0){
        Temperature = AppData::getData(messageRecv);

        ui->labTemp->setText(QString::number(Temperature) + "℃");
        ui->labPower->setText(QString::number(Power) + "%");

        QFile file(csvFile);
        file.open( QIODevice::Append );
        QTextStream out(&file);
        out<<tr("%1,").arg(count/2)<<tr("%2,").arg(Temperature)<<tr("%3,\n").arg(Power);
        file.close();

        QChart *qchart =(QChart *)ui->graphicsView->chart();
        QLineSeries *ser0 = (QLineSeries *)ui->graphicsView->chart()->series().at(0);
        QLineSeries *ser1 = (QLineSeries *)ui->graphicsView->chart()->series().at(1);

        //更新数据
        ser0->append(count/2 ,Temperature);
        ser1->append(count/2 ,Power);

        if (count%20 == 0)
            qchart->axisX()->setMax(10 + (count/2));

        messageRecv.clear();
        processData("R01020");
        serial->write(messageSend);
    }
    else {
        Power = AppData::getData(messageRecv);
        messageRecv.clear();
        processData("R02800");
        serial->write(messageSend);
    }
    count ++;

}

void MainWindow::on_OpenSerialButton_clicked()
{
    if(serial->isOpen())                                        // 如果串口打开了，先给他关闭
    {
        serial->clear();
        serial->close();
        // 关闭状态，按钮显示“打开串口”
        ui->OpenSerialButton->setText("  打开串口");
        // 禁止操作“发送字符操作”
        ui->btnGetTemp->setDisabled(true);
        // 关闭状态，颜色为绿色
        ui->OpenSerialButton->setStyleSheet("color: green;");
        // 关闭，显示灯为红色
        LED(true);
    }
    else                                                        // 如果串口关闭了，先给他打开
    {
        //当前选择的串口名字
        serial->setPortName(ui->PortBox->currentText());
        //用ReadWrite 的模式尝试打开串口，无法收发数据时，发出警告
        if(!serial->open(QIODevice::ReadWrite))
        {
            QMessageBox::warning(this,tr("提示"),tr("串口打开失败!"),QMessageBox::Ok);
            return;
         }
        // 打开状态，按钮显示“关闭串口”
        ui->OpenSerialButton->setText("  关闭串口");
        // 允许操作“发送字符操作”
        ui->btnGetTemp->setDisabled(false);
        // 打开状态，颜色为红色
        ui->OpenSerialButton->setStyleSheet("color: red;");
        // 打开，显示灯为绿色
        LED(false);
    }
}

void MainWindow::on_btnGetTemp_clicked()
{
    csvFile = QFileDialog::getExistingDirectory();

    QDateTime current_date_time =QDateTime::currentDateTime();
    QString current_date =current_date_time.toString("yyyy.MM.dd.hh.mm.ss");
    csvFile += tr("/%1_SR23.csv").arg(current_date);

    QFile file(csvFile);
    file.open( QIODevice::ReadWrite | QIODevice::Text );

    QTextStream out(&file);
    out<<tr("time,")<<tr("temperature,")<<tr("power,\n");

    file.close();

    timer->start(500);
}
