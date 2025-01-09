/********************************************************************************
** Form generated from reading UI file 'connexion.ui'
**
** Created by: Qt User Interface Compiler version 5.15.13
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CONNEXION_H
#define UI_CONNEXION_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>

QT_BEGIN_NAMESPACE

class Ui_Connexion
{
public:
    QPushButton *connexionButton;
    QLineEdit *loginLineEdit;

    void setupUi(QDialog *Connexion)
    {
        if (Connexion->objectName().isEmpty())
            Connexion->setObjectName(QString::fromUtf8("Connexion"));
        Connexion->resize(268, 234);
        connexionButton = new QPushButton(Connexion);
        connexionButton->setObjectName(QString::fromUtf8("connexionButton"));
        connexionButton->setGeometry(QRect(90, 140, 80, 25));
        loginLineEdit = new QLineEdit(Connexion);
        loginLineEdit->setObjectName(QString::fromUtf8("loginLineEdit"));
        loginLineEdit->setGeometry(QRect(70, 110, 121, 25));

        retranslateUi(Connexion);

        QMetaObject::connectSlotsByName(Connexion);
    } // setupUi

    void retranslateUi(QDialog *Connexion)
    {
        Connexion->setWindowTitle(QCoreApplication::translate("Connexion", "Dialog", nullptr));
        connexionButton->setText(QCoreApplication::translate("Connexion", "Connexion", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Connexion: public Ui_Connexion {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CONNEXION_H
