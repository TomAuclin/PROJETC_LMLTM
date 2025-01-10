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
        // get login
        std::string getLogin();
        ~GestionUtilisateur();
};

#endif