#include <iostream>
#include <string>
#include "GestionUtilisateur.hpp"

GestionUtilisateur::GestionUtilisateur() : login("") {}

void GestionUtilisateur::connexion() {
    if (login == "ad-01-ao" || login == "us-02-al") {
        std::cout << "Connexion réussie." << std::endl;
    } else {
        std::cout << "Connexion échouée." << std::endl;
    }
}


void GestionUtilisateur::deconnexion() {
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
}
