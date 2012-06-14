#ifndef DIALOGSETCOM_H
#define DIALOGSETCOM_H

#include <QDialog>
#include "qextserialbase.h"

namespace Ui {
    class DialogSetCom;
}

class DialogSetCom : public QDialog
{
    Q_OBJECT

public:
    explicit DialogSetCom(QWidget *parent = 0);
    ~DialogSetCom();
signals:
    void emitComInfo(PortSettings setting,QString com);



private:
    Ui::DialogSetCom *ui;

private slots:
    void on_pushButtonCancel_clicked();
    void on_pushButtonSet_clicked();
};

#endif // DIALOGSETCOM_H
