#include "Connexion.h"
#include "ui_Connexion.h"
#include <QMessageBox>
#include <QDebug>

Connexion::Connexion(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Connexion),
    biblioWindow(nullptr)
{
    ui->setupUi(this);
    connect(ui->connexionButton, &QPushButton::clicked, this, &Connexion::on_connexionButton_clicked);
}



void Connexion::on_connexionButton_clicked()
{
    QString login = ui->loginLineEdit->text();
    qDebug() << "Login saisi: " << login;

    gestionUtilisateur.setLogin(login.toStdString());
    gestionUtilisateur.connexion(); // Appelle la méthode corrigée

    if (login == "ad-01-ao" || login == "us-02-al") {
        qDebug() << "Connexion réussie";

        if (!biblioWindow) {
            biblioWindow = std::make_unique<BiblioWindow>();
            biblioWindow->loadImagesFromDirectory("/media/sf_PROJETC_LMLTM/Bibliotheque/Gratuites");
        }

        biblioWindow->show();
        close();
    } else {
        qDebug() << "Connexion échouée";
        QMessageBox::warning(this, "Erreur", "Identification échouée, essayez encore.");
        ui->loginLineEdit->clear(); // Permet de saisir un nouveau mot de passe
    }
}


Connexion::~Connexion()
{
    delete ui;
}