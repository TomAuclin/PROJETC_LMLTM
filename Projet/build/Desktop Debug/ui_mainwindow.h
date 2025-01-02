/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.15.13
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QGraphicsView>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QPushButton *ChargerImage;
    QGraphicsView *AfficherImage;
    QGraphicsView *AfficherHisto;
    QPushButton *CalculerHisto;
    QCheckBox *Canal_R;
    QCheckBox *Canal_V;
    QCheckBox *Canal_B;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(842, 690);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        ChargerImage = new QPushButton(centralwidget);
        ChargerImage->setObjectName(QString::fromUtf8("ChargerImage"));
        ChargerImage->setGeometry(QRect(20, 30, 121, 25));
        AfficherImage = new QGraphicsView(centralwidget);
        AfficherImage->setObjectName(QString::fromUtf8("AfficherImage"));
        AfficherImage->setGeometry(QRect(280, 0, 321, 241));
        AfficherHisto = new QGraphicsView(centralwidget);
        AfficherHisto->setObjectName(QString::fromUtf8("AfficherHisto"));
        AfficherHisto->setGeometry(QRect(70, 250, 681, 311));
        CalculerHisto = new QPushButton(centralwidget);
        CalculerHisto->setObjectName(QString::fromUtf8("CalculerHisto"));
        CalculerHisto->setGeometry(QRect(20, 70, 121, 25));
        Canal_R = new QCheckBox(centralwidget);
        Canal_R->setObjectName(QString::fromUtf8("Canal_R"));
        Canal_R->setGeometry(QRect(120, 560, 101, 23));
        Canal_V = new QCheckBox(centralwidget);
        Canal_V->setObjectName(QString::fromUtf8("Canal_V"));
        Canal_V->setGeometry(QRect(240, 560, 91, 23));
        Canal_B = new QCheckBox(centralwidget);
        Canal_B->setObjectName(QString::fromUtf8("Canal_B"));
        Canal_B->setGeometry(QRect(350, 560, 91, 23));
        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName(QString::fromUtf8("menubar"));
        menubar->setGeometry(QRect(0, 0, 842, 22));
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName(QString::fromUtf8("statusbar"));
        MainWindow->setStatusBar(statusbar);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "MainWindow", nullptr));
        ChargerImage->setText(QCoreApplication::translate("MainWindow", "ChargerImage", nullptr));
        CalculerHisto->setText(QCoreApplication::translate("MainWindow", "CalculerHisto", nullptr));
        Canal_R->setText(QCoreApplication::translate("MainWindow", "Canal Rouge", nullptr));
        Canal_V->setText(QCoreApplication::translate("MainWindow", "Canal Vert", nullptr));
        Canal_B->setText(QCoreApplication::translate("MainWindow", "Canal Bleu", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
