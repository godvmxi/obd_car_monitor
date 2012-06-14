#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileDialog>
#include <QDebug>
#include <QMessageBox>
#include "dialogsetcom.h"
#include "qextserialbase.h"
#include <QTextCursor>
#include <QTextBrowser>
#include "sysdef.h"

#include "win_qextserialport.h"


#define MAX_FILE_SIZE  1024000  //1M
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
    void getComInfo(PortSettings setting,QString com);

private:
    Ui::MainWindow *ui;
    QString fileName;
    QFile   fileHandler;
    bool isHexFile;
    bool isFileSellected;
    bool isFileOpened;
    qint64 offset;
    qint32 sizeOfFrame;
    qint64 sizeOfFile;
    uint16_t crcOfBin;
    uint16_t packetNum;
    QDataStream dataStreamHandler;
    PortSettings comSetting;
    QString comName;
    Win_QextSerialPort *myCom;

    uint32_t dealAskStartMsg(IAP_START *iapBrife);
    uint32_t dealAskDataMsg(IAP_START *iapBrife);
    uint32_t dealRegisterMsg(char *point);

    bool    getFrame(bool direct);
    char binfile[MAX_FILE_SIZE];

private slots:
    void on_action_exit_triggered();
    void on_pushButtonSend_clicked();
    void on_action_disconnect_triggered();
    void on_action_connect_triggered();
    void on_serial_set_triggered();
    void on_spinBoxBufSize_valueChanged(int );
    void on_checkBoxHex_stateChanged(int );
    void on_open_bin_triggered();
    void on_open_hex_triggered();
    void readReady(void);

};

#endif // MAINWINDOW_H
