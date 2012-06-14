#include "dialogsetcom.h"
#include "ui_dialogsetcom.h"
#include <QDebug>

DialogSetCom::DialogSetCom(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogSetCom)
{

    ui->setupUi(this);
    ui->comboBoxDatabit->setCurrentIndex(3);
    ui->comboBoxBaud->setCurrentIndex(2);
}

DialogSetCom::~DialogSetCom()
{
    delete ui;
}

void DialogSetCom::on_pushButtonSet_clicked()
{
    struct PortSettings port;
 //   qDebug()<<ui->comboBoxBaud->currentText();
    switch(ui->comboBoxBaud->currentText().toInt()){
        case 9600:
            port.BaudRate = BAUD9600;
            break;
        case 38400:
            port.BaudRate = BAUD38400;
            break;
        case 115200:
            port.BaudRate = BAUD115200;
            break;
        case 256000:
            port.BaudRate = BAUD256000;
            break;
        default :
            qDebug()<<"baud rates error";
            break;
    }
    qDebug()<<"baud rate:"<<port.BaudRate;
    //data bits
    switch(ui->comboBoxDatabit->currentIndex()){
        case 0 :
               port.DataBits =  DATA_5;
               break;
        case 1 :
               port.DataBits =  DATA_6;
               break;
        case 2 :
               port.DataBits =  DATA_7;
               break;
        case 3 :
               port.DataBits =  DATA_8;
               break;
        default :
                qDebug()<<"data bits error";
                return;
    }
    //stop
    switch(ui->comboBoxStopbit->currentIndex()){
        case 0 :
               port.StopBits =  STOP_1;
               break;
        case 1 :
               port.StopBits =  STOP_1_5;
               break;
        case 2 :
               port.StopBits =  STOP_2;
               break;
        default :
                qDebug()<<"stop bits error";
                return;
    }
    //Flow
    switch(ui->comboBoxFlow->currentIndex()){
        case 0 :
               port.FlowControl =  FLOW_OFF;
               break;
        case 1 :
               port.FlowControl =  FLOW_HARDWARE;
               break;
        case 2 :
               port.FlowControl =  FLOW_XONXOFF;
               break;
        default :
                qDebug()<<"data bits error";
                return;
    }
    //Parity
    switch(ui->comboBoxCrc->currentIndex()){
        case 0 :
               port.Parity =  PAR_NONE;
               break;
        case 1 :
               port.Parity =  PAR_ODD;
               break;
        case 2 :
               port.Parity =  PAR_EVEN;
               break;
        case 3 :
               port.Parity =  PAR_MARK;
               break;
        case 4 :
               port.Parity =  PAR_SPACE;
               break;
        default :
                qDebug()<<"data bits error";
                return;
    }

//    port.DataBits = ui->comboBoxDatabit->currentIndex();
//    port.StopBits = ui->comboBoxStopbit->currentIndex();
//    port.FlowControl = ui->comboBoxFlow->currentIndex();
//    port.Parity = ui->comboBoxCrc->currentIndex();

    qDebug()<<"data bits :"<<port.DataBits;
    qDebug()<<"stop bits :"<<port.StopBits;
    qDebug()<<"flow ctrl :"<<port.FlowControl;
    qDebug()<<"parity    :"<<port.Parity;


    QString com = ui->comboBoxPort->currentText();
    port.Timeout_Millisec = ui->spinBoxTimeOut->value();
    qDebug()<<port.Timeout_Millisec;
    emit emitComInfo(port,com);
    this->accept();
  //  qDebug()<<com;

}

void DialogSetCom::on_pushButtonCancel_clicked()
{
    this->reject();
}
