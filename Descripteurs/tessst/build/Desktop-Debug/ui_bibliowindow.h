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
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QGraphicsView>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_BiblioWindow
{
public:
    QAction *actionCharger_la_Bibliotheque;
    QWidget *centralwidget;
    QGraphicsView *AffichageBiblio;
    QMenuBar *menubar;
    QMenu *menuFichier;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *BiblioWindow)
    {
        if (BiblioWindow->objectName().isEmpty())
            BiblioWindow->setObjectName(QString::fromUtf8("BiblioWindow"));
        BiblioWindow->resize(800, 600);
        actionCharger_la_Bibliotheque = new QAction(BiblioWindow);
        actionCharger_la_Bibliotheque->setObjectName(QString::fromUtf8("actionCharger_la_Bibliotheque"));
        centralwidget = new QWidget(BiblioWindow);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        AffichageBiblio = new QGraphicsView(centralwidget);
        AffichageBiblio->setObjectName(QString::fromUtf8("AffichageBiblio"));
        AffichageBiblio->setGeometry(QRect(100, 60, 621, 351));
        BiblioWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(BiblioWindow);
        menubar->setObjectName(QString::fromUtf8("menubar"));
        menubar->setGeometry(QRect(0, 0, 800, 22));
        menuFichier = new QMenu(menubar);
        menuFichier->setObjectName(QString::fromUtf8("menuFichier"));
        BiblioWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(BiblioWindow);
        statusbar->setObjectName(QString::fromUtf8("statusbar"));
        BiblioWindow->setStatusBar(statusbar);

        menubar->addAction(menuFichier->menuAction());
        menuFichier->addAction(actionCharger_la_Bibliotheque);

        retranslateUi(BiblioWindow);

        QMetaObject::connectSlotsByName(BiblioWindow);
    } // setupUi

    void retranslateUi(QMainWindow *BiblioWindow)
    {
        BiblioWindow->setWindowTitle(QCoreApplication::translate("BiblioWindow", "MainWindow", nullptr));
        actionCharger_la_Bibliotheque->setText(QCoreApplication::translate("BiblioWindow", "Charger la Bibliotheque", nullptr));
        menuFichier->setTitle(QCoreApplication::translate("BiblioWindow", "Fichier", nullptr));
    } // retranslateUi

};

namespace Ui {
    class BiblioWindow: public Ui_BiblioWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_BIBLIOWINDOW_H
