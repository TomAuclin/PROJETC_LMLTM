/********************************************************************************
** Form generated from reading UI file 'bibliowindow.ui'
**
** Created by: Qt User Interface Compiler version 5.15.13
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_BIBLIOWINDOW_H
#define UI_BIBLIOWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_BiblioWindow
{
public:
    QWidget *centralwidget;
    QPushButton *ChargerBiblioButton;
    QPushButton *ChargeBoutton;
    QPushButton *SaveBoutton;
    QPushButton *DetailsButton;
    QListWidget *AffichageBiblio;
    QPushButton *TraitementButton;
    QPushButton *pushButtonRechercherp;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *BiblioWindow)
    {
        if (BiblioWindow->objectName().isEmpty())
            BiblioWindow->setObjectName(QString::fromUtf8("BiblioWindow"));
        BiblioWindow->resize(936, 521);
        centralwidget = new QWidget(BiblioWindow);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        ChargerBiblioButton = new QPushButton(centralwidget);
        ChargerBiblioButton->setObjectName(QString::fromUtf8("ChargerBiblioButton"));
        ChargerBiblioButton->setGeometry(QRect(40, 10, 171, 25));
        ChargeBoutton = new QPushButton(centralwidget);
        ChargeBoutton->setObjectName(QString::fromUtf8("ChargeBoutton"));
        ChargeBoutton->setGeometry(QRect(50, 500, 100, 30));
        SaveBoutton = new QPushButton(centralwidget);
        SaveBoutton->setObjectName(QString::fromUtf8("SaveBoutton"));
        SaveBoutton->setGeometry(QRect(160, 500, 100, 30));
        DetailsButton = new QPushButton(centralwidget);
        DetailsButton->setObjectName(QString::fromUtf8("DetailsButton"));
        DetailsButton->setGeometry(QRect(810, 40, 80, 21));
        AffichageBiblio = new QListWidget(centralwidget);
        AffichageBiblio->setObjectName(QString::fromUtf8("AffichageBiblio"));
        AffichageBiblio->setGeometry(QRect(10, 40, 781, 431));
        TraitementButton = new QPushButton(centralwidget);
        TraitementButton->setObjectName(QString::fromUtf8("TraitementButton"));
        TraitementButton->setGeometry(QRect(810, 80, 80, 25));
        pushButtonRechercherp = new QPushButton(centralwidget);
        pushButtonRechercherp->setObjectName(QString::fromUtf8("pushButtonRechercherp"));
        pushButtonRechercherp->setGeometry(QRect(810, 0, 80, 25));
        BiblioWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(BiblioWindow);
        menubar->setObjectName(QString::fromUtf8("menubar"));
        menubar->setGeometry(QRect(0, 0, 936, 22));
        BiblioWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(BiblioWindow);
        statusbar->setObjectName(QString::fromUtf8("statusbar"));
        BiblioWindow->setStatusBar(statusbar);

        retranslateUi(BiblioWindow);

        QMetaObject::connectSlotsByName(BiblioWindow);
    } // setupUi

    void retranslateUi(QMainWindow *BiblioWindow)
    {
        BiblioWindow->setWindowTitle(QCoreApplication::translate("BiblioWindow", "MainWindow", nullptr));
        ChargerBiblioButton->setText(QCoreApplication::translate("BiblioWindow", "Charger biblio", nullptr));
        ChargeBoutton->setText(QCoreApplication::translate("BiblioWindow", "Charger", nullptr));
        SaveBoutton->setText(QCoreApplication::translate("BiblioWindow", "Sauvegarder", nullptr));
        DetailsButton->setText(QCoreApplication::translate("BiblioWindow", "Details", nullptr));
        TraitementButton->setText(QCoreApplication::translate("BiblioWindow", "Traitement", nullptr));
        pushButtonRechercherp->setText(QCoreApplication::translate("BiblioWindow", "Prix Image", nullptr));
    } // retranslateUi

};

namespace Ui {
    class BiblioWindow: public Ui_BiblioWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_BIBLIOWINDOW_H
