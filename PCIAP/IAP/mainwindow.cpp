#include "mainwindow.h"
#include "ui_mainwindow.h"
class DialogSetCom;
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    this->isFileSellected = false;
    this->sizeOfFrame = 512;

    ui->setupUi(this);
    ui->action_connect->setEnabled(false);
    ui->action_disconnect->setEnabled(false);

    this->setIconSize(QSize(64,64));

    QString ack("####AAAAAAAA|");
    ack.append(ui->lineEdit_iap_ip->text());
    ack.append("|");
    ack.append(ui->lineEdit_iap_port->text());
    ack.append("|");
    ack.append(ui->lineEdit_net_ip->text());
    ack.append("|");
    ack.append(ui->lineEdit_net_port->text());
    ack.append("|");
    ack.append(ui->lineEdit_blue_name->text());
    ack.append("|");
    ack.append(ui->lineEdit_blue_pass->text());
    ack.append("|");
    ack.append(ui->comboBox_buad->currentText());
    ack.append("*");
    qDebug()<<"cmd :"<<ack;

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_open_hex_triggered()
{
    QString file = QFileDialog::getOpenFileName(
            this,
            "open HEX file",
            QDir::currentPath(),
            "Document files (*.hex)"
            );
    if(file.isNull()){
        qDebug()<<"no hex selected";
        this->isFileSellected = false;
        return ;
    }
    else{
        qDebug()<<"file name :"<<file;
        this->isHexFile = true;
        this->isFileSellected = true;
        this->fileName = file;
        sizeOfFile = QFileInfo(fileName).size();
        qDebug()<<"file size : "<< sizeOfFile;
//        dataStreamHandler.setDevice(this->fileHandler);
    }
}

void MainWindow::on_open_bin_triggered()
{
    QString file = QFileDialog::getOpenFileName(
            this,
            tr("open BIN file"),
            QDir::currentPath(),
            "Document files (*.bin)"
            );
    if(file.isNull()){
        qDebug()<<"no bin file selected";
        this->isFileSellected = false;
        return ;
    }
    else{
        qDebug()<<"file name :"<<file;
        this->isHexFile = false;
        this->isFileSellected = true;
        this->fileName = file;
        QFileInfo info(this->fileName);
        this->sizeOfFile = info.size();
        qDebug()<<"file size is :"<<this->sizeOfFile;
        this->fileHandler.setFileName(this->fileName);
        if(fileHandler.open(QIODevice::ReadOnly) == false){
            qDebug()<<"open file " <<fileName<<" error";
            this->isFileSellected = false;
            return ;
        }
        else{
             qDebug()<<"open file ok";

             quint32 tmp = this->fileHandler.read(this->binfile,MAX_FILE_SIZE);
             qDebug()<<"file size 1:"<<tmp<<"  read size:"<<tmp;
             this->crcOfBin = calBufCrc((uint8_t *)this->binfile,tmp);
             qDebug()<<"file crc:"<<this->crcOfBin;
             if(tmp != this->sizeOfFile)
             {
                  qDebug()<<"read file seiz";
                  return ;
             }
             this->packetNum = tmp/512;
             if(tmp%512 != 0)
                    this->packetNum++;
             qDebug()<<"packet num is :" <<this->packetNum;
            QString tmp1("welcome to use pc iap");
//             QString tmp2;
//             for(int i = 0;i<tmp;i++){
//                 tmp2.clear();
//                 if(i %16 == 0){
//                      tmp2.append("\n");
//                  }
//                 if(i %512 == 0){
//                     uint32_t size = tmp - i;
//                     if(size >= 512)
//                         size = 512;
//                     uint16_t crcTmp = calBufCrc((uint8_t *)(this->binfile + i),size);
//                      tmp2.sprintf("\n\nfile index %d: crc is : %3X\n\n",i/512+1,crcTmp);

//                  }
//                tmp1.append(tmp2);
//                tmp2.sprintf("%4X",quint8(this->binfile[i]));
//                tmp1.append(tmp2);

//             }
             ui->textBrowser->insertPlainText(tmp1);
        }
    }
}

void MainWindow::on_checkBoxHex_stateChanged(int )
{
    return ;
}

void MainWindow::on_spinBoxBufSize_valueChanged(int value)
{
    this->sizeOfFrame = value;
    qDebug()<<"Frame size : "<<value;
}
bool MainWindow::getFrame(bool direct){
//    char buf[10000];

    if(direct){
        qDebug()<<"get next frame";
        if((this->sizeOfFile - this->offset) < this->sizeOfFrame ){
            QMessageBox(QMessageBox::Warning, "file over", "file is over!!!").exec();
            return false;
        }
        else{

   //         this->dataStreamHandler.readBytes(buf,this->sizeOfFrame);
//            for(int i=0;i<this->sizeOfFrame,i++){
 //               ui->textBrowser->append(QString());
  //              ui->textBrowser->append(" ");
 //           }

        }



    }
}

void MainWindow::on_serial_set_triggered()
{
    DialogSetCom *setCom = new DialogSetCom;
    connect(setCom,SIGNAL(emitComInfo(PortSettings,QString)),this,SLOT(getComInfo(PortSettings,QString)));
    setCom->exec();
    delete(setCom);

}
void MainWindow:: getComInfo(PortSettings setting,QString com){
    qDebug()<<setting.BaudRate;
    qDebug()<<com;
    this->comSetting = setting;
    this->comName = com;
    ui->action_connect->setEnabled(true);

}



void MainWindow::on_action_connect_triggered()
{
    ui->action_connect->setEnabled(false);
    ui->action_disconnect->setEnabled(true);
    myCom = new Win_QextSerialPort(this->comName,this->comSetting,QextSerialBase::EventDriven);
    if(myCom->open(QIODevice::ReadWrite) < 0)
           qDebug()<<"open error";
    myCom->setBaudRate(BAUD115200);
    myCom->setDataBits(DATA_8);
    myCom->setParity(PAR_NONE);
    myCom->setStopBits(STOP_1);
    myCom->setFlowControl(FLOW_OFF);
    myCom->setTimeout(500);
    connect(myCom,SIGNAL(readyRead()),this,SLOT(readReady()));


}

void MainWindow::on_action_disconnect_triggered()
{
    ui->action_connect->setEnabled(true);
    ui->action_disconnect->setEnabled(false);
    myCom->close();
    delete(myCom);
}
void MainWindow::readReady(void){
    int i = myCom->bytesAvailable();
    if(i < 16){
        return;
    }
//    qDebug()<<"available data :"<<i;
    QByteArray temp = myCom->readAll();

    QTextCursor cursor = ui->textBrowser->textCursor();
    cursor.movePosition(QTextCursor::End);
    ui->textBrowser->setTextCursor(cursor);

    ui->textBrowser->insertPlainText(temp);
 //   qDebug()<<"data in";


    char *buf;
    char *point= NULL;
    char *point2 = NULL;

    buf = temp.data();
    point = strstr(buf,"##");
    if(point != NULL){
        IAP_START *iapStart = (IAP_START *)(point);
//        qDebug()<<"find ##";
        if(iapStart->crc == calBufCrc((uint8_t *)point +4,sizeof(IAP_START)-4)){
            qDebug()<<"Find the IAP_BRIFE AND CRC OK";
            if(iapStart->typeOrIndex == 0){
                qDebug()<<"FIND THE ASK START MSG : "<<iapStart->typeOrIndex;
                dealAskStartMsg(iapStart);
            }
            else
            {
                qDebug()<<"FIND THE ASK DATA MSG : "<<iapStart->typeOrIndex;
                dealAskDataMsg(iapStart);
            }

        }
        else{
            point = strstr(buf,"####");
            if(point != NULL)
            {
                ui->textBrowser->insertPlainText(tr("\nfind device register information\n"));
                this->dealRegisterMsg(point);


            }
        }
    }




}

void MainWindow::on_pushButtonSend_clicked()
{
    myCom->write(ui->lineEditSend->text().toAscii());
}
uint32_t MainWindow::dealAskStartMsg(IAP_START *iapStart){
    IAP_BRIFE iapBrife;
    iapBrife.start[0] = '#';
    iapBrife.start[1] = '#';
    iapBrife.sizeOfApp = this->sizeOfFile;
    iapBrife.typeOrIndex = -1;
    iapBrife.hv = 1;
    iapBrife.sv = 1;

    iapBrife.packetNum = this->packetNum;

    iapBrife.appCrc =calBufCrc((uint8_t *)this->binfile,this->sizeOfFile);
    iapBrife.crc = calBufCrc((uint8_t *)iapBrife.start +4,sizeof(IAP_BRIFE)-4);
//    qDebug()<<"packet size :"<<iapBrife.sizeOfApp<<"  num: "<<iapBrife.packetNum<<"  appcrc: "<<iapBrife.appCrc<<"  crc :"<<iapBrife.crc;
    QByteArray sendData((char *)(iapBrife.start),sizeof(IAP_BRIFE));

    QString tmp;
    tmp.sprintf("\n\nindex :%d len :%d  crc :%5X appSize:%d appCrc :%5X packetNum :%d \n\n",iapBrife.typeOrIndex,sizeof(IAP_BRIFE),iapBrife.crc,iapBrife.appCrc,iapBrife.packetNum,iapBrife);
    qDebug()<<tmp;
    QTextCursor cursor = ui->textBrowser->textCursor();
    cursor.movePosition(QTextCursor::End);
    ui->textBrowser->setTextCursor(cursor);

    ui->textBrowser->insertPlainText(tmp);
    qDebug()<<sendData.toHex();
    this->myCom->write(sendData);

    return 1;
}

uint32_t MainWindow::dealAskDataMsg(IAP_START *iapStart){
    IAP_DATA iapData;


    iapData.start[0] = '#';
    iapData.start[1] = '#';
    iapData.hv = 1;
    iapData.sv = 1;
    iapData.typeOrIndex  = iapStart->typeOrIndex;
    iapData.app[0] = '*';
    iapData.app[1] = '*';
    iapData.app[2] = '*';
    iapData.app[3] = '*';
    iapData.packetLength = 512;
    if(iapStart->typeOrIndex == this->packetNum)
        iapData.packetLength = this->sizeOfFile - (iapData.typeOrIndex-1)*512;

    iapData.packetCrc = calBufCrc((uint8_t *)(this->binfile + (iapData.typeOrIndex-1)*512 ),iapData.packetLength);

    iapData.crc = calBufCrc((uint8_t *)iapData.start+4,sizeof(IAP_DATA)-4);
    QString tmp;
    tmp.sprintf("\n\nindex :%d len :%d  crc :%5X\n\n",iapData.typeOrIndex,iapData.packetLength,iapData.crc);
    qDebug()<<tmp;
    QTextCursor cursor = ui->textBrowser->textCursor();
    cursor.movePosition(QTextCursor::End);
    ui->textBrowser->setTextCursor(cursor);

    ui->textBrowser->insertPlainText(tmp);

 //   qDebug()<<"index:"<<iapData.typeOrIndex<<" packet len:"<<iapData.packetLength<<"  packet crc:"<<iapData.packetCrc;

    QByteArray sendData((char *)iapData.start,sizeof(IAP_DATA));
    sendData.append((char *)(this->binfile+(iapData.typeOrIndex-1)*512),iapData.packetLength);
    this->myCom->write(sendData);
    return 1;
}

void MainWindow::on_action_exit_triggered()
{
    this->close();
}
uint32_t MainWindow::dealRegisterMsg(char *point)
{
    QString ack("####AAAAAAAA|");
    ack.append(ui->lineEdit_iap_ip->text());
    ack.append("|");
    ack.append(ui->lineEdit_iap_port->text());
    ack.append("|");
    ack.append(ui->lineEdit_net_ip->text());
    ack.append("|");
    ack.append(ui->lineEdit_net_port->text());
    ack.append("|");
    ack.append(ui->lineEdit_blue_name->text());
    ack.append("|");
    ack.append(ui->lineEdit_blue_pass->text());
    ack.append("|");
    ack.append(ui->comboBox_buad->currentText());
    ack.append("*");
    qDebug()<<"cmd :"<<ack;

    myCom->write(ack.toAscii());

}
