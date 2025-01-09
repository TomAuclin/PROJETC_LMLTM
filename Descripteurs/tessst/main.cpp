#include "mainwindow.h"
#include "bibliowindow.h"
#include "connexion.h"
#include "GestionUtilisateur.hpp"
#include <QDebug>
#include <QApplication>


int main(int argc, char *argv[])
{

    QApplication a(argc, argv);

    MainWindow w;
    w.show();

    // Créer la fenêtre de connexion
    Connexion loginDialog;

    // Ouvrir la fenêtre de connexion et attendre qu'elle soit fermée (succès ou échec)
    loginDialog.exec();  // Exécuter la fenêtre de connexion, le programme s'arrête ici jusqu'à ce qu'elle soit fermée

    // Si la connexion échoue, aucune autre fenêtre ne sera ouverte
    // Si la connexion réussit, la fenêtre Bibliothèque sera ouverte dans le slot `on_connexionButton_clicked()`

    return a.exec();
}

