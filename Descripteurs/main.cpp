#include "Library.hpp"
#include "Library.cpp"
#include "Image.hpp"
#include "Image.cpp"
#include <iostream>

#include "GestionUtilisateur.hpp"
#include "GestionUtilisateur.cpp"


void afficherImagesAvecAccesO(const std::string& filename);

int main() {
    Library liste;
    GestionUtilisateur user;

    Library listetest;

    listetest.ajouterDescripteurs(Image("Lucas", "Bruno.png", 130, 20000000000, 'O', "gris", 4, 130));
    
    liste.chargerDepuisFichier("Biblio_init.txt");


    // Connexion

    // si connexion echouer alors recommencer une connexion (Admin/User)
    while (user.getLogin() != "ad-01-ao" && user.getLogin() != "us-02-al") {
        user.connexion();
    }

    // boucle seulement pour si us-02-al le user est connecté
    if (user.getLogin() == "us-02-al") {
        std::cout << "Vous êtes connecter en tant que simple user (acces restreint)." << std::endl;
        liste.afficherImagesAvecAccesO("Biblio_init.txt");
    }

    // boucle seulement pour si ad-01-ao l'admin est connecté
    if (user.getLogin() == "ad-01-ao") {
        std::cout << "Vous êtes connecter en tant qu'Administrateur (tout les acces)." << std::endl;
        //liste.afficher();

        // Recherche d'une image par numéro
        //liste.chargerDepuisFichier("Biblio_init.txt");
        //liste.afficher();

        // la recherche n'est pas dans le txt ????
        //liste.tricroissant(liste);    
        //std::cout << "La taille de la liste est de : " << liste.tailleListe() << std::endl;


        int numeroRecherche;
        std::cout << "Entrez le numero de l'image a rechercher : ";
        std::cin >> numeroRecherche;
        std::string resultat = liste.rechercherImageParNumero(numeroRecherche);
        std::cout << resultat << std::endl; 

        //liste.afficher(); 
        //liste.sauvegarderDansFichier("Biblio_init.txt");

        // modification de l'acces d'une image
        int numeroModification;
        std::cout << "Entrez le numéro de l'image à modifier l'accès : ";
        std::cin >> numeroModification;

        // Appel de la méthode pour modifier l'accès dans la liste
        liste.modifierAcces(numeroModification,"Biblio_test.txt");
        // Afficher l'image numéro 30
        std::cout << liste.rechercherImageParNumero(30) << std::endl;

        liste.sousListe(liste);
    }




     //liste.tricroissant(liste);
    
    // liste.afficher();
     // std::cout << "La taille de la liste est de : " << liste.tailleListe() << std::endl;
     // std::string resultat = liste.rechercherImageParNumero(numeroRecherche);
     // std::cout << resultat << std::endl; 
      //liste.afficher();
      liste.sauvegarderDansFichier("Biblio_init.txt");
    


   // liste.supprimerDescripteurs(113);
    liste.afficher();
    //liste.modifierAccesDansFichier("Biblio_init.txt", 113);
    liste.afficherImagesAvecAccesO("Biblio_init.txt");
    return 0;
}