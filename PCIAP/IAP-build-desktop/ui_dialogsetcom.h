/********************************************************************************
** Form generated from reading UI file 'dialogsetcom.ui'
**
** Created: Tue Jun 5 16:12:21 2012
**      by: Qt User Interface Compiler version 4.7.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DIALOGSETCOM_H
#define UI_DIALOGSETCOM_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QComboBox>
#include <QtGui/QDialog>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QPushButton>
#include <QtGui/QSpinBox>

QT_BEGIN_NAMESPACE

class Ui_DialogSetCom
{
public:
    QComboBox *comboBoxPort;
    QComboBox *comboBoxBaud;
    QComboBox *comboBoxDatabit;
    QComboBox *comboBoxStopbit;
    QLabel *label;
    QLabel *label_2;
    QLabel *label_3;
    QLabel *label_4;
    QComboBox *comboBoxCrc;
    QLabel *label_5;
    QPushButton *pushButtonSet;
    QPushButton *pushButtonCancel;
    QComboBox *comboBoxFlow;
    QLabel *label_6;
    QSpinBox *spinBoxTimeOut;
    QLabel *label_7;

    void setupUi(QDialog *DialogSetCom)
    {
        if (DialogSetCom->objectName().isEmpty())
            DialogSetCom->setObjectName(QString::fromUtf8("DialogSetCom"));
        DialogSetCom->resize(400, 300);
        comboBoxPort = new QComboBox(DialogSetCom);
        comboBoxPort->setObjectName(QString::fromUtf8("comboBoxPort"));
        comboBoxPort->setGeometry(QRect(80, 40, 69, 22));
        comboBoxBaud = new QComboBox(DialogSetCom);
        comboBoxBaud->setObjectName(QString::fromUtf8("comboBoxBaud"));
        comboBoxBaud->setGeometry(QRect(250, 40, 69, 22));
        comboBoxDatabit = new QComboBox(DialogSetCom);
        comboBoxDatabit->setObjectName(QString::fromUtf8("comboBoxDatabit"));
        comboBoxDatabit->setGeometry(QRect(80, 120, 69, 22));
        comboBoxStopbit = new QComboBox(DialogSetCom);
        comboBoxStopbit->setObjectName(QString::fromUtf8("comboBoxStopbit"));
        comboBoxStopbit->setGeometry(QRect(250, 120, 69, 22));
        label = new QLabel(DialogSetCom);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(10, 30, 61, 31));
        QFont font;
        font.setFamily(QString::fromUtf8("\345\256\213\344\275\223"));
        font.setPointSize(12);
        font.setBold(false);
        font.setWeight(50);
        label->setFont(font);
        label_2 = new QLabel(DialogSetCom);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setGeometry(QRect(170, 30, 61, 31));
        label_2->setFont(font);
        label_3 = new QLabel(DialogSetCom);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        label_3->setGeometry(QRect(10, 120, 61, 31));
        label_3->setFont(font);
        label_4 = new QLabel(DialogSetCom);
        label_4->setObjectName(QString::fromUtf8("label_4"));
        label_4->setGeometry(QRect(170, 120, 61, 31));
        label_4->setFont(font);
        comboBoxCrc = new QComboBox(DialogSetCom);
        comboBoxCrc->setObjectName(QString::fromUtf8("comboBoxCrc"));
        comboBoxCrc->setGeometry(QRect(80, 190, 69, 22));
        label_5 = new QLabel(DialogSetCom);
        label_5->setObjectName(QString::fromUtf8("label_5"));
        label_5->setGeometry(QRect(10, 180, 61, 31));
        label_5->setFont(font);
        pushButtonSet = new QPushButton(DialogSetCom);
        pushButtonSet->setObjectName(QString::fromUtf8("pushButtonSet"));
        pushButtonSet->setGeometry(QRect(70, 260, 75, 23));
        QFont font1;
        font1.setPointSize(12);
        pushButtonSet->setFont(font1);
        pushButtonCancel = new QPushButton(DialogSetCom);
        pushButtonCancel->setObjectName(QString::fromUtf8("pushButtonCancel"));
        pushButtonCancel->setGeometry(QRect(240, 260, 75, 23));
        pushButtonCancel->setFont(font1);
        comboBoxFlow = new QComboBox(DialogSetCom);
        comboBoxFlow->setObjectName(QString::fromUtf8("comboBoxFlow"));
        comboBoxFlow->setGeometry(QRect(250, 190, 69, 22));
        label_6 = new QLabel(DialogSetCom);
        label_6->setObjectName(QString::fromUtf8("label_6"));
        label_6->setGeometry(QRect(180, 180, 61, 31));
        label_6->setFont(font);
        spinBoxTimeOut = new QSpinBox(DialogSetCom);
        spinBoxTimeOut->setObjectName(QString::fromUtf8("spinBoxTimeOut"));
        spinBoxTimeOut->setGeometry(QRect(80, 230, 71, 22));
        spinBoxTimeOut->setMaximum(10000);
        spinBoxTimeOut->setSingleStep(50);
        spinBoxTimeOut->setValue(300);
        label_7 = new QLabel(DialogSetCom);
        label_7->setObjectName(QString::fromUtf8("label_7"));
        label_7->setGeometry(QRect(20, 230, 61, 31));
        label_7->setFont(font);

        retranslateUi(DialogSetCom);

        QMetaObject::connectSlotsByName(DialogSetCom);
    } // setupUi

    void retranslateUi(QDialog *DialogSetCom)
    {
        DialogSetCom->setWindowTitle(QApplication::translate("DialogSetCom", "Dialog", 0, QApplication::UnicodeUTF8));
        comboBoxPort->clear();
        comboBoxPort->insertItems(0, QStringList()
         << QApplication::translate("DialogSetCom", "COM5", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("DialogSetCom", "COM1", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("DialogSetCom", "COM2", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("DialogSetCom", "COM3", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("DialogSetCom", "COM4", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("DialogSetCom", "COM6", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("DialogSetCom", "COM7", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("DialogSetCom", "COM8", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("DialogSetCom", "COM9", 0, QApplication::UnicodeUTF8)
        );
        comboBoxBaud->clear();
        comboBoxBaud->insertItems(0, QStringList()
         << QApplication::translate("DialogSetCom", "9600", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("DialogSetCom", "38400", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("DialogSetCom", "115200", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("DialogSetCom", "230400", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("DialogSetCom", "256000", 0, QApplication::UnicodeUTF8)
        );
        comboBoxDatabit->clear();
        comboBoxDatabit->insertItems(0, QStringList()
         << QApplication::translate("DialogSetCom", "5", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("DialogSetCom", "6", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("DialogSetCom", "7", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("DialogSetCom", "8", 0, QApplication::UnicodeUTF8)
        );
        comboBoxStopbit->clear();
        comboBoxStopbit->insertItems(0, QStringList()
         << QApplication::translate("DialogSetCom", "1", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("DialogSetCom", "1.5", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("DialogSetCom", "2", 0, QApplication::UnicodeUTF8)
        );
        label->setText(QApplication::translate("DialogSetCom", "\347\253\257\345\217\243", 0, QApplication::UnicodeUTF8));
        label_2->setText(QApplication::translate("DialogSetCom", "\346\263\242\347\211\271\347\216\207", 0, QApplication::UnicodeUTF8));
        label_3->setText(QApplication::translate("DialogSetCom", "\346\225\260\346\215\256\344\275\215", 0, QApplication::UnicodeUTF8));
        label_4->setText(QApplication::translate("DialogSetCom", "\345\201\234\346\255\242\344\275\215", 0, QApplication::UnicodeUTF8));
        comboBoxCrc->clear();
        comboBoxCrc->insertItems(0, QStringList()
         << QApplication::translate("DialogSetCom", "NONE", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("DialogSetCom", "ODD", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("DialogSetCom", "EVEN", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("DialogSetCom", "MARK", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("DialogSetCom", "SPACE", 0, QApplication::UnicodeUTF8)
        );
        label_5->setText(QApplication::translate("DialogSetCom", "\346\240\241\351\252\214\344\275\215", 0, QApplication::UnicodeUTF8));
        pushButtonSet->setText(QApplication::translate("DialogSetCom", "\350\256\276\347\275\256", 0, QApplication::UnicodeUTF8));
        pushButtonCancel->setText(QApplication::translate("DialogSetCom", "\345\217\226\346\266\210", 0, QApplication::UnicodeUTF8));
        comboBoxFlow->clear();
        comboBoxFlow->insertItems(0, QStringList()
         << QApplication::translate("DialogSetCom", "OFF", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("DialogSetCom", "HARD", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("DialogSetCom", "XONXOFF", 0, QApplication::UnicodeUTF8)
        );
        label_6->setText(QApplication::translate("DialogSetCom", "\346\265\201\346\216\247", 0, QApplication::UnicodeUTF8));
        label_7->setText(QApplication::translate("DialogSetCom", "\350\266\205\346\227\266", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class DialogSetCom: public Ui_DialogSetCom {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DIALOGSETCOM_H
