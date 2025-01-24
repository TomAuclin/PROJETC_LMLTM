#ifndef GESTIONUTILISATEUR_HPP
#define GESTIONUTILISATEUR_HPP

// Inclusion des fichiers nécessaires pour gérer l'interface, les images et la bibliothèque.
#include <string>

class GestionUtilisateur {
private:
    // Stocke le login de l'utilisateur sous forme de chaîne de caractères
    std::string login;

public:
    // Constructeur de la classe GestionUtilisateur
    GestionUtilisateur();

    void connexion(); // Méthode pour connecter l'utilisateur
    void deconnexion(); // Méthode pour déconnecter l'utilisateur
    void setLogin(const std::string& login); // Méthode pour mettre à jour le login de l'utilisateur

    std::string getLogin();  // Méthode pour obtenir le login actuel de l'utilisateur

    // Destructeur de la classe GestionUtilisateur
    ~GestionUtilisateur();
};


#endif // GESTIONUTILISATEUR_HPP
