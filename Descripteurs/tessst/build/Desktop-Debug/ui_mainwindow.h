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
#include <QtWidgets/QGroupBox>
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
    QGraphicsView *AffichageResultat;
    QPushButton *CalculerHisto;
    QCheckBox *Canal_R;
    QCheckBox *Canal_V;
    QCheckBox *Canal_B;
    QPushButton *DetectionContour;
    QPushButton *DetectionDroite;
    QPushButton *AppliquerConvolution;
    QGroupBox *GroupeSegmentation;
    QCheckBox *Segmentation_Canal_R;
    QCheckBox *Segmentation_Canal_V;
    QCheckBox *Segmentation_Canal_B;
    QPushButton *SegmenterCouleur;
    QPushButton *RetourVersBiblio;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(1299, 700);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        ChargerImage = new QPushButton(centralwidget);
        ChargerImage->setObjectName(QString::fromUtf8("ChargerImage"));
        ChargerImage->setGeometry(QRect(20, 10, 181, 25));
        AfficherImage = new QGraphicsView(centralwidget);
        AfficherImage->setObjectName(QString::fromUtf8("AfficherImage"));
        AfficherImage->setGeometry(QRect(220, 0, 461, 421));
        AffichageResultat = new QGraphicsView(centralwidget);
        AffichageResultat->setObjectName(QString::fromUtf8("AffichageResultat"));
        AffichageResultat->setGeometry(QRect(690, 0, 461, 421));
        CalculerHisto = new QPushButton(centralwidget);
        CalculerHisto->setObjectName(QString::fromUtf8("CalculerHisto"));
        CalculerHisto->setGeometry(QRect(20, 110, 181, 25));
        Canal_R = new QCheckBox(centralwidget);
        Canal_R->setObjectName(QString::fromUtf8("Canal_R"));
        Canal_R->setGeometry(QRect(740, 430, 100, 23));
        Canal_V = new QCheckBox(centralwidget);
        Canal_V->setObjectName(QString::fromUtf8("Canal_V"));
        Canal_V->setGeometry(QRect(880, 430, 100, 23));
        Canal_B = new QCheckBox(centralwidget);
        Canal_B->setObjectName(QString::fromUtf8("Canal_B"));
        Canal_B->setGeometry(QRect(1010, 430, 100, 23));
        DetectionContour = new QPushButton(centralwidget);
        DetectionContour->setObjectName(QString::fromUtf8("DetectionContour"));
        DetectionContour->setGeometry(QRect(20, 150, 181, 25));
        DetectionDroite = new QPushButton(centralwidget);
        DetectionDroite->setObjectName(QString::fromUtf8("DetectionDroite"));
        DetectionDroite->setGeometry(QRect(20, 190, 181, 25));
        AppliquerConvolution = new QPushButton(centralwidget);
        AppliquerConvolution->setObjectName(QString::fromUtf8("AppliquerConvolution"));
        AppliquerConvolution->setGeometry(QRect(20, 230, 181, 25));
        GroupeSegmentation = new QGroupBox(centralwidget);
        GroupeSegmentation->setObjectName(QString::fromUtf8("GroupeSegmentation"));
        GroupeSegmentation->setGeometry(QRect(20, 270, 171, 161));
        Segmentation_Canal_R = new QCheckBox(GroupeSegmentation);
        Segmentation_Canal_R->setObjectName(QString::fromUtf8("Segmentation_Canal_R"));
        Segmentation_Canal_R->setGeometry(QRect(10, 20, 150, 25));
        Segmentation_Canal_V = new QCheckBox(GroupeSegmentation);
        Segmentation_Canal_V->setObjectName(QString::fromUtf8("Segmentation_Canal_V"));
        Segmentation_Canal_V->setGeometry(QRect(10, 50, 150, 25));
        Segmentation_Canal_B = new QCheckBox(GroupeSegmentation);
        Segmentation_Canal_B->setObjectName(QString::fromUtf8("Segmentation_Canal_B"));
        Segmentation_Canal_B->setGeometry(QRect(10, 80, 150, 25));
        SegmenterCouleur = new QPushButton(GroupeSegmentation);
        SegmenterCouleur->setObjectName(QString::fromUtf8("SegmenterCouleur"));
        SegmenterCouleur->setGeometry(QRect(10, 120, 150, 30));
        RetourVersBiblio = new QPushButton(centralwidget);
        RetourVersBiblio->setObjectName(QString::fromUtf8("RetourVersBiblio"));
        RetourVersBiblio->setGeometry(QRect(80, 470, 80, 25));
        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName(QString::fromUtf8("menubar"));
        menubar->setGeometry(QRect(0, 0, 1299, 22));
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName(QString::fromUtf8("statusbar"));
        MainWindow->setStatusBar(statusbar);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "Application de Traitement d'Images", nullptr));
        ChargerImage->setText(QCoreApplication::translate("MainWindow", "Charger Image", nullptr));
        CalculerHisto->setText(QCoreApplication::translate("MainWindow", "Calculer Histogramme", nullptr));
        Canal_R->setText(QCoreApplication::translate("MainWindow", "Canal Rouge", nullptr));
        Canal_V->setText(QCoreApplication::translate("MainWindow", "Canal Vert", nullptr));
        Canal_B->setText(QCoreApplication::translate("MainWindow", "Canal Bleu", nullptr));
        DetectionContour->setText(QCoreApplication::translate("MainWindow", "D\303\251tection Contours", nullptr));
        DetectionDroite->setText(QCoreApplication::translate("MainWindow", "D\303\251tection Droites", nullptr));
        AppliquerConvolution->setText(QCoreApplication::translate("MainWindow", "Appliquer Convolution", nullptr));
        GroupeSegmentation->setTitle(QCoreApplication::translate("MainWindow", "Segmentation Couleur", nullptr));
        Segmentation_Canal_R->setText(QCoreApplication::translate("MainWindow", "Rouge", nullptr));
        Segmentation_Canal_V->setText(QCoreApplication::translate("MainWindow", "Vert", nullptr));
        Segmentation_Canal_B->setText(QCoreApplication::translate("MainWindow", "Bleu", nullptr));
        SegmenterCouleur->setText(QCoreApplication::translate("MainWindow", "Segmenter", nullptr));
        RetourVersBiblio->setText(QCoreApplication::translate("MainWindow", "Retour", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
