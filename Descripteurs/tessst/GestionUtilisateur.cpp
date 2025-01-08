<<<<<<< HEAD
#include <iostream>
#include <string>
=======
// Classe gestion utilisateur, attribut : login (string), et 2 methodes : connexion et deconnexion

#include <iostream>
#include <string>

>>>>>>> 3de15a3f379c2af1909b49ffb40e671860ef70b3
#include "GestionUtilisateur.hpp"

GestionUtilisateur::GestionUtilisateur() : login("") {}

void GestionUtilisateur::connexion() {
<<<<<<< HEAD
    // Si login "ad-01-ao" ou "us-02-al" alors connexion réussie
    while (login != "ad-01-ao" && login != "us-02-al") {
=======
    // si login "Admin" ou "User" alors connection réussi sinon non
    // si connection echouer alros refaire une connexion
    while (login != "ad-01-ao" && login != "us-02-al") {
        std::cout << "Entrez votre login : ";
        std::cin >> login;
>>>>>>> 3de15a3f379c2af1909b49ffb40e671860ef70b3
        if (login == "ad-01-ao" || login == "us-02-al") {
            std::cout << "Connexion réussie." << std::endl;
        } else {
            std::cout << "Connexion échouée." << std::endl;
        }
    }
}

void GestionUtilisateur::deconnexion() {
<<<<<<< HEAD
    // Se déconnecter
    std::cout << "Déconnexion de " << login << std::endl;
    login = "";
}

void GestionUtilisateur::setLogin(const std::string& newLogin) {
    login = newLogin;
}

std::string GestionUtilisateur::getLogin() {
    return login;
}

GestionUtilisateur::~GestionUtilisateur() {
    // Destructeur de la classe GestionUtilisateur
=======
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

>>>>>>> 3de15a3f379c2af1909b49ffb40e671860ef70b3
}
