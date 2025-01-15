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
#include <QtWidgets/QLabel>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_BiblioWindow
{
public:
    QWidget *centralwidget;
    QPushButton *ChargeBoutton;
    QPushButton *SaveBoutton;
    QPushButton *DetailsButton;
    QListWidget *AffichageBiblio;
    QPushButton *TraitementButton;
    QPushButton *pushButtonRechercherp;
    QTextEdit *textEdit;
    QLabel *labelImageCount;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *BiblioWindow)
    {
        if (BiblioWindow->objectName().isEmpty())
            BiblioWindow->setObjectName(QString::fromUtf8("BiblioWindow"));
        BiblioWindow->resize(936, 587);
        centralwidget = new QWidget(BiblioWindow);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        ChargeBoutton = new QPushButton(centralwidget);
        ChargeBoutton->setObjectName(QString::fromUtf8("ChargeBoutton"));
        ChargeBoutton->setGeometry(QRect(20, 10, 161, 30));
        SaveBoutton = new QPushButton(centralwidget);
        SaveBoutton->setObjectName(QString::fromUtf8("SaveBoutton"));
        SaveBoutton->setGeometry(QRect(280, 490, 100, 30));
        DetailsButton = new QPushButton(centralwidget);
        DetailsButton->setObjectName(QString::fromUtf8("DetailsButton"));
        DetailsButton->setGeometry(QRect(810, 90, 80, 21));
        AffichageBiblio = new QListWidget(centralwidget);
        AffichageBiblio->setObjectName(QString::fromUtf8("AffichageBiblio"));
        AffichageBiblio->setGeometry(QRect(10, 50, 781, 431));
        TraitementButton = new QPushButton(centralwidget);
        TraitementButton->setObjectName(QString::fromUtf8("TraitementButton"));
        TraitementButton->setGeometry(QRect(810, 130, 80, 25));
        pushButtonRechercherp = new QPushButton(centralwidget);
        pushButtonRechercherp->setObjectName(QString::fromUtf8("pushButtonRechercherp"));
        pushButtonRechercherp->setGeometry(QRect(810, 50, 80, 25));
        textEdit = new QTextEdit(centralwidget);
        textEdit->setObjectName(QString::fromUtf8("textEdit"));
        textEdit->setGeometry(QRect(200, 10, 141, 31));
        labelImageCount = new QLabel(centralwidget);
        labelImageCount->setObjectName(QString::fromUtf8("labelImageCount"));
        labelImageCount->setGeometry(QRect(470, 20, 151, 17));
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
        ChargeBoutton->setText(QCoreApplication::translate("BiblioWindow", "Charger la Biblioth\303\250que", nullptr));
        SaveBoutton->setText(QCoreApplication::translate("BiblioWindow", "Sauvegarder", nullptr));
        DetailsButton->setText(QCoreApplication::translate("BiblioWindow", "Details", nullptr));
        TraitementButton->setText(QCoreApplication::translate("BiblioWindow", "Traitement", nullptr));
        pushButtonRechercherp->setText(QCoreApplication::translate("BiblioWindow", "Prix Image", nullptr));
        textEdit->setHtml(QCoreApplication::translate("BiblioWindow", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><meta charset=\"utf-8\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"hr { height: 1px; border-width: 0; }\n"
"li.unchecked::marker { content: \"\\2610\"; }\n"
"li.checked::marker { content: \"\\2612\"; }\n"
"</style></head><body style=\" font-family:'Sans Serif'; font-size:9pt; font-weight:400; font-style:normal;\">\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" color:#ff0000;\">Charger un fichier.txt</span></p></body></html>", nullptr));
        labelImageCount->setText(QCoreApplication::translate("BiblioWindow", "Nombre d'images : 0", nullptr));
    } // retranslateUi

};

namespace Ui {
    class BiblioWindow: public Ui_BiblioWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_BIBLIOWINDOW_H
