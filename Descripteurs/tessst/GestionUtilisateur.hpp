#ifndef GESTIONUTILISATEUR_HPP
#define GESTIONUTILISATEUR_HPP

#include <string>

class GestionUtilisateur {
private:
    std::string login;
public:
    GestionUtilisateur();
    void connexion();
    void deconnexion();
    void setLogin(const std::string& login);  // Méthode pour mettre à jour le login
    std::string getLogin();
    ~GestionUtilisateur();
};

#endif // GESTIONUTILISATEUR_HPP
