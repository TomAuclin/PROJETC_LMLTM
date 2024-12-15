// Classe gestion utilisateur, attribut : login (string), et 2 methodes : connexion et deconnexion

#include <iostream>
#include <string>

#include "GestionUtilisateur.hpp"

GestionUtilisateur::GestionUtilisateur() : login("") {}

void GestionUtilisateur::connexion() {
    // si login "Admin" ou "User" alors connection réussi sinon non
    // si connection echouer alros refaire une connexion
    while (login != "Admin" && login != "User") {
        std::cout << "Entrez votre login : ";
        std::cin >> login;
        if (login == "Admin" || login == "User") {
            std::cout << "Connexion reussie." << std::endl;
        } else {
            std::cout << "Connexion echouee." << std::endl;
        }
    }

}

void GestionUtilisateur::deconnexion() {
    // se déconnecter
    std::cout << "Déconnexion de " << login << std::endl;
    login = "";

}

std::string GestionUtilisateur::getLogin()
{
    // retourne le login
    return login;
}

GestionUtilisateur::~GestionUtilisateur()
{
    // destructeur de la classe GestionUtilisateur

}
