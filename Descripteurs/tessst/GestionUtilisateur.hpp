#ifndef GESTIONUTILISATEUR_HPP
#define GESTIONUTILISATEUR_HPP

#include <string>

<<<<<<< HEAD
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
=======

class GestionUtilisateur {
    private:
        std::string login;
    public:
        GestionUtilisateur();
        void connexion();
        void deconnexion();
        // get login
        std::string getLogin();
        ~GestionUtilisateur();
};

#endif
>>>>>>> 3de15a3f379c2af1909b49ffb40e671860ef70b3
