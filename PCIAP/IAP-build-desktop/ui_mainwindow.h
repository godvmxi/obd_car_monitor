/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created: Wed Jun 6 12:32:19 2012
**      by: Qt User Interface Compiler version 4.7.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QCheckBox>
#include <QtGui/QComboBox>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QMainWindow>
#include <QtGui/QMenu>
#include <QtGui/QMenuBar>
#include <QtGui/QPushButton>
#include <QtGui/QSpinBox>
#include <QtGui/QStatusBar>
#include <QtGui/QTextBrowser>
#include <QtGui/QToolBar>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QAction *serial_set;
    QAction *open_hex;
    QAction *open_bin;
    QAction *action_last;
    QAction *action_next;
    QAction *action_connect;
    QAction *action_disconnect;
    QAction *action_exit;
    QWidget *centralWidget;
    QCheckBox *checkBoxHex;
    QSpinBox *spinBoxBufSize;
    QWidget *verticalLayoutWidget;
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout;
    QLineEdit *lineEditSend;
    QPushButton *pushButtonSend;
    QTextBrowser *textBrowser;
    QLineEdit *lineEdit_iap_ip;
    QLineEdit *lineEdit_iap_port;
    QLineEdit *lineEdit_net_port;
    QLineEdit *lineEdit_net_ip;
    QLabel *label;
    QLabel *label_2;
    QLabel *label_3;
    QLineEdit *lineEdit;
    QLineEdit *lineEdit_blue_name;
    QLabel *label_5;
    QLineEdit *lineEdit_blue_pass;
    QComboBox *comboBox_buad;
    QMenuBar *menuBar;
    QMenu *menu;
    QMenu *menu_2;
    QMenu *menu_3;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(587, 760);
        serial_set = new QAction(MainWindow);
        serial_set->setObjectName(QString::fromUtf8("serial_set"));
        open_hex = new QAction(MainWindow);
        open_hex->setObjectName(QString::fromUtf8("open_hex"));
        open_bin = new QAction(MainWindow);
        open_bin->setObjectName(QString::fromUtf8("open_bin"));
        action_last = new QAction(MainWindow);
        action_last->setObjectName(QString::fromUtf8("action_last"));
        action_next = new QAction(MainWindow);
        action_next->setObjectName(QString::fromUtf8("action_next"));
        action_connect = new QAction(MainWindow);
        action_connect->setObjectName(QString::fromUtf8("action_connect"));
        action_disconnect = new QAction(MainWindow);
        action_disconnect->setObjectName(QString::fromUtf8("action_disconnect"));
        action_exit = new QAction(MainWindow);
        action_exit->setObjectName(QString::fromUtf8("action_exit"));
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        checkBoxHex = new QCheckBox(centralWidget);
        checkBoxHex->setObjectName(QString::fromUtf8("checkBoxHex"));
        checkBoxHex->setGeometry(QRect(500, 10, 70, 17));
        spinBoxBufSize = new QSpinBox(centralWidget);
        spinBoxBufSize->setObjectName(QString::fromUtf8("spinBoxBufSize"));
        spinBoxBufSize->setEnabled(true);
        spinBoxBufSize->setGeometry(QRect(420, 10, 61, 22));
        spinBoxBufSize->setMaximum(2048);
        spinBoxBufSize->setSingleStep(512);
        spinBoxBufSize->setValue(512);
        verticalLayoutWidget = new QWidget(centralWidget);
        verticalLayoutWidget->setObjectName(QString::fromUtf8("verticalLayoutWidget"));
        verticalLayoutWidget->setGeometry(QRect(10, 90, 571, 591));
        verticalLayout = new QVBoxLayout(verticalLayoutWidget);
        verticalLayout->setSpacing(6);
        verticalLayout->setContentsMargins(11, 11, 11, 11);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(6);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        lineEditSend = new QLineEdit(verticalLayoutWidget);
        lineEditSend->setObjectName(QString::fromUtf8("lineEditSend"));

        horizontalLayout->addWidget(lineEditSend);

        pushButtonSend = new QPushButton(verticalLayoutWidget);
        pushButtonSend->setObjectName(QString::fromUtf8("pushButtonSend"));

        horizontalLayout->addWidget(pushButtonSend);


        verticalLayout->addLayout(horizontalLayout);

        textBrowser = new QTextBrowser(verticalLayoutWidget);
        textBrowser->setObjectName(QString::fromUtf8("textBrowser"));

        verticalLayout->addWidget(textBrowser);

        lineEdit_iap_ip = new QLineEdit(centralWidget);
        lineEdit_iap_ip->setObjectName(QString::fromUtf8("lineEdit_iap_ip"));
        lineEdit_iap_ip->setGeometry(QRect(40, 10, 131, 20));
        lineEdit_iap_ip->setFocusPolicy(Qt::StrongFocus);
        lineEdit_iap_port = new QLineEdit(centralWidget);
        lineEdit_iap_port->setObjectName(QString::fromUtf8("lineEdit_iap_port"));
        lineEdit_iap_port->setGeometry(QRect(190, 10, 51, 20));
        lineEdit_net_port = new QLineEdit(centralWidget);
        lineEdit_net_port->setObjectName(QString::fromUtf8("lineEdit_net_port"));
        lineEdit_net_port->setGeometry(QRect(190, 50, 51, 20));
        lineEdit_net_ip = new QLineEdit(centralWidget);
        lineEdit_net_ip->setObjectName(QString::fromUtf8("lineEdit_net_ip"));
        lineEdit_net_ip->setGeometry(QRect(40, 50, 131, 20));
        label = new QLabel(centralWidget);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(10, 10, 46, 13));
        label_2 = new QLabel(centralWidget);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setGeometry(QRect(10, 50, 46, 13));
        label_3 = new QLabel(centralWidget);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        label_3->setGeometry(QRect(260, 10, 46, 13));
        lineEdit = new QLineEdit(centralWidget);
        lineEdit->setObjectName(QString::fromUtf8("lineEdit"));
        lineEdit->setGeometry(QRect(320, 10, 51, 20));
        lineEdit_blue_name = new QLineEdit(centralWidget);
        lineEdit_blue_name->setObjectName(QString::fromUtf8("lineEdit_blue_name"));
        lineEdit_blue_name->setGeometry(QRect(320, 10, 51, 20));
        label_5 = new QLabel(centralWidget);
        label_5->setObjectName(QString::fromUtf8("label_5"));
        label_5->setGeometry(QRect(260, 50, 46, 13));
        lineEdit_blue_pass = new QLineEdit(centralWidget);
        lineEdit_blue_pass->setObjectName(QString::fromUtf8("lineEdit_blue_pass"));
        lineEdit_blue_pass->setGeometry(QRect(320, 50, 51, 20));
        comboBox_buad = new QComboBox(centralWidget);
        comboBox_buad->setObjectName(QString::fromUtf8("comboBox_buad"));
        comboBox_buad->setGeometry(QRect(420, 50, 69, 22));
        MainWindow->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(MainWindow);
        menuBar->setObjectName(QString::fromUtf8("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 587, 21));
        menu = new QMenu(menuBar);
        menu->setObjectName(QString::fromUtf8("menu"));
        menu_2 = new QMenu(menuBar);
        menu_2->setObjectName(QString::fromUtf8("menu_2"));
        menu_3 = new QMenu(menuBar);
        menu_3->setObjectName(QString::fromUtf8("menu_3"));
        MainWindow->setMenuBar(menuBar);
        mainToolBar = new QToolBar(MainWindow);
        mainToolBar->setObjectName(QString::fromUtf8("mainToolBar"));
        MainWindow->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(MainWindow);
        statusBar->setObjectName(QString::fromUtf8("statusBar"));
        MainWindow->setStatusBar(statusBar);

        menuBar->addAction(menu_2->menuAction());
        menuBar->addAction(menu->menuAction());
        menuBar->addAction(menu_3->menuAction());
        menu->addAction(serial_set);
        menu->addAction(action_connect);
        menu->addAction(action_disconnect);
        menu_2->addAction(open_hex);
        menu_2->addAction(open_bin);
        menu_2->addAction(action_exit);
        menu_3->addAction(action_last);
        menu_3->addAction(action_next);
        mainToolBar->addAction(open_bin);
        mainToolBar->addAction(serial_set);
        mainToolBar->addAction(action_connect);
        mainToolBar->addAction(action_disconnect);

        retranslateUi(MainWindow);

        comboBox_buad->setCurrentIndex(1);


        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "MainWindow", 0, QApplication::UnicodeUTF8));
        serial_set->setText(QApplication::translate("MainWindow", "\344\270\262\345\217\243\350\256\276\347\275\256", 0, QApplication::UnicodeUTF8));
        open_hex->setText(QApplication::translate("MainWindow", "\346\211\223\345\274\200hex\346\226\207\344\273\266", 0, QApplication::UnicodeUTF8));
        open_bin->setText(QApplication::translate("MainWindow", "\346\211\223\345\274\200bin\346\226\207\344\273\266", 0, QApplication::UnicodeUTF8));
        action_last->setText(QApplication::translate("MainWindow", "\344\270\212\344\270\200\345\270\247", 0, QApplication::UnicodeUTF8));
        action_next->setText(QApplication::translate("MainWindow", "\344\270\213\344\270\200\345\270\247", 0, QApplication::UnicodeUTF8));
        action_connect->setText(QApplication::translate("MainWindow", "\350\277\236\346\216\245", 0, QApplication::UnicodeUTF8));
        action_disconnect->setText(QApplication::translate("MainWindow", "\346\226\255\345\274\200", 0, QApplication::UnicodeUTF8));
        action_exit->setText(QApplication::translate("MainWindow", "\351\200\200\345\207\272", 0, QApplication::UnicodeUTF8));
        checkBoxHex->setText(QApplication::translate("MainWindow", "16\350\277\233\345\210\266", 0, QApplication::UnicodeUTF8));
        pushButtonSend->setText(QApplication::translate("MainWindow", "send", 0, QApplication::UnicodeUTF8));
        lineEdit_iap_ip->setText(QApplication::translate("MainWindow", "in.phonewingstech.com", 0, QApplication::UnicodeUTF8));
        lineEdit_iap_port->setText(QApplication::translate("MainWindow", "9000", 0, QApplication::UnicodeUTF8));
        lineEdit_net_port->setText(QApplication::translate("MainWindow", "9001", 0, QApplication::UnicodeUTF8));
        lineEdit_net_ip->setText(QApplication::translate("MainWindow", "in.phonewingstech.com", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("MainWindow", "IAP", 0, QApplication::UnicodeUTF8));
        label_2->setText(QApplication::translate("MainWindow", "NET", 0, QApplication::UnicodeUTF8));
        label_3->setText(QApplication::translate("MainWindow", "\350\223\235\347\211\231\345\220\215\347\247\260", 0, QApplication::UnicodeUTF8));
        lineEdit->setText(QApplication::translate("MainWindow", "OBD_3", 0, QApplication::UnicodeUTF8));
        lineEdit_blue_name->setText(QApplication::translate("MainWindow", "OBD_3", 0, QApplication::UnicodeUTF8));
        label_5->setText(QApplication::translate("MainWindow", "\350\223\235\347\211\231\345\257\206\347\240\201", 0, QApplication::UnicodeUTF8));
        lineEdit_blue_pass->setText(QApplication::translate("MainWindow", "1234", 0, QApplication::UnicodeUTF8));
        comboBox_buad->clear();
        comboBox_buad->insertItems(0, QStringList()
         << QApplication::translate("MainWindow", "9600", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("MainWindow", "38400", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("MainWindow", "115200", 0, QApplication::UnicodeUTF8)
        );
        menu->setTitle(QApplication::translate("MainWindow", "\350\256\276\347\275\256", 0, QApplication::UnicodeUTF8));
        menu_2->setTitle(QApplication::translate("MainWindow", "\346\223\215\344\275\234", 0, QApplication::UnicodeUTF8));
        menu_3->setTitle(QApplication::translate("MainWindow", "\346\226\207\344\273\266\346\237\245\347\234\213", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
