// Inclusion des fichiers d'en-tête
#include "mainwindow.h"
#include "bibliowindow.h"
#include "connexionwindow.h"

// Inclusion des fichiers nécessaires pour gérer l'interface, les images et la bibliothèque.
#include <QApplication>

int main(int argc, char *argv[])
{
    // Création de l'objet QApplication pour initialiser l'environnement Qt
    QApplication a(argc, argv);

    // Création d'une instance de la fenêtre de connexion (ConnexionWindow)
    ConnexionWindow user;

    // Affichage de la fenêtre de connexion
    user.show();

    // Lancement de la boucle principale de l'application Qt
    return a.exec();
}
