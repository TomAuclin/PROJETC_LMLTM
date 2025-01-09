// Classe gestion utilisateur, attribut : login (string), et 2 methodes : connexion et deconnexion

#include <iostream>
#include <string>

#include "GestionUtilisateur.hpp"

GestionUtilisateur::GestionUtilisateur() : login("") {}

void GestionUtilisateur::connexion() {
    // si login "Admin" ou "User" alors connection réussi sinon non
    // si connection echouer alros refaire une connexion
    while (login != "ad-01-ao" && login != "us-02-al") {
        std::cout << "Entrez votre login : ";
        std::cin >> login;
        if (login == "ad-01-ao" || login == "us-02-al") {
            std::cout << "Connexion réussie." << std::endl;
        } else {
            std::cout << "Connexion échouée." << std::endl;
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
