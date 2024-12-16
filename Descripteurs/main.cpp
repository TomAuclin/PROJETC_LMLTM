#include "Library.hpp"
#include "Library.cpp"
#include "Image.hpp"
#include "Image.cpp"
#include <iostream>

#include "GestionUtilisateur.hpp"
#include "GestionUtilisateur.cpp"

// Function declaration
void afficherImagesAvecAccesO(const std::string& filename);

int main() {
    Library liste;
    GestionUtilisateur user;

     /// Gratuites :
    liste.ajouterDescripteurs(Image("OpenSources", "Baboon_couleur.png", 1, 0, 'O', "couleur", 5, 1));
    liste.ajouterDescripteurs(Image("OpenSources", "Baboon_gris.pgm", 2, 0, 'O', "gris", 4, 2));
    liste.ajouterDescripteurs(Image("OpenSources", "Barbara_couleur.png", 3, 0, 'O', "couleur", 4, 3));
    liste.ajouterDescripteurs(Image("OpenSources", "Barbara_gris.pgm", 4, 0, 'O', "gris", 3, 4));
    liste.ajouterDescripteurs(Image("OpenSources", "Bird_couleur.png", 5, 0, 'O', "couleur", 5, 5));
    liste.ajouterDescripteurs(Image("OpenSources", "Bird_gris.pgm", 6, 0, 'O', "gris", 4, 6));
    liste.ajouterDescripteurs(Image("Tom", "EauToitPont_couleur.CR2", 7, 0, 'O', "couleur", 5, 7));
    liste.ajouterDescripteurs(Image("Tom", "EauToitPont_gris.pgm", 8, 0, 'O', "gris", 4, 8));
    liste.ajouterDescripteurs(Image("Tom", "Geek_couleur.CR2", 9, 0, 'O', "couleur", 4, 9));
    liste.ajouterDescripteurs(Image("Tom", "Geek_gris.pgm", 10, 0, 'O', "gris", 3, 10));
    liste.ajouterDescripteurs(Image("OpenSources", "ImageAerienne_couleur.png", 11, 0, 'O', "couleur", 6, 11));
    liste.ajouterDescripteurs(Image("OpenSources", "ImageAerienne_gris.pgm", 12, 0, 'O', "gris", 5, 12));
    liste.ajouterDescripteurs(Image("OpenSources", "Lena_couleur.png", 13, 0, 'O', "couleur", 5, 13));
    liste.ajouterDescripteurs(Image("OpenSources", "Lena_gris.pgm", 14, 0, 'O', "gris", 4, 14));
    liste.ajouterDescripteurs(Image("OpenSources", "Micro1_gris.png", 15, 0, 'O', "gris", 5, 15));
    liste.ajouterDescripteurs(Image("OpenSources", "Cameraman_gris.pgm", 16, 0, 'O', "gris", 5, 16));
    liste.ajouterDescripteurs(Image("OpenSources", "Facade_gris.pgm", 17, 0, 'O', "gris", 5, 17));


    // <9.99€ :
    liste.ajouterDescripteurs(Image("Tom", "ArbreMultiBranche_couleur.CR2", 18, 8, 'L', "couleur", 4, 18));
    liste.ajouterDescripteurs(Image("Tom", "ArbreMultiBranche_gris.pgm", 19, 7, 'L', "gris", 4, 19));
    liste.ajouterDescripteurs(Image("Tom", "Banc_02_couleur.CR2", 20, 8, 'L', "couleur", 5, 20));
    liste.ajouterDescripteurs(Image("Tom", "Banc_02_gris.pgm", 21, 6, 'L', "gris", 5, 21));
    liste.ajouterDescripteurs(Image("Tom", "BandeSol_couleur.CR2", 22, 5, 'L', "couleur", 3, 22));
    liste.ajouterDescripteurs(Image("Tom", "BandeSol_gris.pgm", 23, 2, 'L', "gris", 3, 23));
    liste.ajouterDescripteurs(Image("Tom", "Bateau01_couleur.CR2", 24, 9, 'L', "couleur", 6, 24));
    liste.ajouterDescripteurs(Image("Tom", "Bateau01_gris.pgm", 25, 7, 'L', "gris", 5, 25));
    liste.ajouterDescripteurs(Image("Lydia", "BateauCote_couleur.CR2", 26, 9, 'L', "couleur", 6, 26));
    liste.ajouterDescripteurs(Image("Lydia", "BateauCote_gris.pgm", 27, 9, 'L', "gris", 5, 27));    
    liste.ajouterDescripteurs(Image("Tom", "BriqueMur_couleur.CR2", 28, 8, 'L', "couleur", 6, 28));
    liste.ajouterDescripteurs(Image("Tom", "BriqueMur_gris.pgm", 29, 3, 'L', "gris", 5, 29)); 
    liste.ajouterDescripteurs(Image("Tom", "CedezPassage01_couleur.CR2", 30, 3, 'L', "couleur", 6, 30));
    liste.ajouterDescripteurs(Image("Tom", "CedezPassage01_gris.pgm", 31, 1, 'L', "gris", 5, 31)); 
    liste.ajouterDescripteurs(Image("Lydia", "GaletSol_couleur.CR2", 32, 9, 'L', "couleur", 4, 32));
    liste.ajouterDescripteurs(Image("Lydia", "GaletSol_gris.pgm", 33, 6, 'L', "gris", 4, 33)); 
    liste.ajouterDescripteurs(Image("Tom", "LimaceVelo_couleur.CR2", 34, 9, 'L', "couleur", 4, 34));
    liste.ajouterDescripteurs(Image("Tom", "LimaceVelo_gris.pgm", 35, 7, 'L', "gris", 3, 35)); 
    liste.ajouterDescripteurs(Image("Tom", "PanneauPieton_couleur.CR2", 36, 3, 'L', "couleur", 6, 36));
    liste.ajouterDescripteurs(Image("Tom", "PanneauPieton_gris.pgm", 37, 1, 'L', "gris", 5, 37)); 
    liste.ajouterDescripteurs(Image("Tom", "Pieton_couleur.CR2", 38, 5, 'L', "couleur", 4, 38));
    liste.ajouterDescripteurs(Image("Tom", "Pieton_gris.pgm", 39, 3, 'L', "gris", 3, 39));
    liste.ajouterDescripteurs(Image("Lydia", "PlanteEau01_couleur.CR2", 40, 9, 'L', "couleur", 5, 40));
    liste.ajouterDescripteurs(Image("Lydia", "PlanteEau01_gris.pgm", 41, 9, 'L', "gris", 4, 41));
    liste.ajouterDescripteurs(Image("Lydia", "PontGauche01_couleur.CR2", 42, 5, 'L', "couleur", 5, 42));
    liste.ajouterDescripteurs(Image("Lydia", "PontGauche01_gris.pgm", 43, 2, 'L', "gris", 4, 43));
    liste.ajouterDescripteurs(Image("Lydia", "SolBois01_couleur.CR2", 44, 4, 'L', "couleur", 5, 44));
    liste.ajouterDescripteurs(Image("Lydia", "SolBois01_gris.pgm", 45, 2, 'L', "gris", 4, 45));
    liste.ajouterDescripteurs(Image("Lydia", "Statu_couleur.CR2", 46, 9, 'L', "couleur", 6, 46));
    liste.ajouterDescripteurs(Image("Lydia", "Statu_gris.pgm", 47, 8, 'L', "gris", 5, 47));
    liste.ajouterDescripteurs(Image("Lydia", "Ver_couleur.CR2", 48, 9, 'L', "couleur", 5, 48));
    liste.ajouterDescripteurs(Image("Lydia", "Ver_gris.pgm", 49, 5, 'L', "gris", 4, 49));
    liste.ajouterDescripteurs(Image("Tom", "Vitraille_couleur.CR2", 50, 6, 'L', "couleur", 6, 50));
    liste.ajouterDescripteurs(Image("Tom", "Vitraille_gris.pgm", 51, 4, 'L', "gris", 5, 51));


    // <99.99€ :
    liste.ajouterDescripteurs(Image("Lydia", "BateauEau01_couleur.CR2", 52, 99, 'L', "couleur", 3, 52));
    liste.ajouterDescripteurs(Image("Lydia", "BateauEau01_gris.pgm", 53, 60, 'L', "gris", 2, 53));
    liste.ajouterDescripteurs(Image("Lydia", "BateauEau04_couleur.CR2", 54, 99, 'L', "couleur", 3, 54));
    liste.ajouterDescripteurs(Image("Lydia", "BateauEau04_gris.pgm", 55, 60, 'L', "gris", 2, 55));
    liste.ajouterDescripteurs(Image("Tom", "BeaucoupVer_couleur.CR2", 56, 81, 'L', "couleur", 3, 56));
    liste.ajouterDescripteurs(Image("Tom", "BeaucoupVer_gris.pgm", 57, 20, 'L', "gris", 2, 57));
    liste.ajouterDescripteurs(Image("Tom", "BriqueSol01_couleur.CR2", 58, 22, 'L', "couleur", 2, 58));
    liste.ajouterDescripteurs(Image("Tom", "BriqueSol01_gris.pgm", 59, 10, 'L', "gris", 2, 59));
    liste.ajouterDescripteurs(Image("Lydia", "BriqueSol02_couleur.CR2", 60, 15, 'L', "couleur", 2, 60));
    liste.ajouterDescripteurs(Image("Lydia", "BriqueSol02_gris.pgm", 61, 10, 'L', "gris", 2, 61));
    liste.ajouterDescripteurs(Image("Tom", "CedezPassage02_couleur.CR2", 62, 41, 'L', "couleur", 6, 62));
    liste.ajouterDescripteurs(Image("Tom", "CedezPassage02_gris.pgm", 63, 20, 'L', "gris", 5, 63));
    liste.ajouterDescripteurs(Image("Tom", "DessousPont01_couleur.CR2", 64, 90, 'L', "couleur", 4, 64));
    liste.ajouterDescripteurs(Image("Tom", "DessousPont01_gris.pgm", 65, 50, 'L', "gris", 4, 65));
    liste.ajouterDescripteurs(Image("Tom", "DomePont_couleur.CR2", 66, 99, 'L', "couleur", 5, 66));
    liste.ajouterDescripteurs(Image("Tom", "DomePont_gris.pgm", 67, 80, 'L', "gris", 4, 67));
    liste.ajouterDescripteurs(Image("Lydia", "Grille01_couleur.CR2", 68, 60, 'L', "couleur", 4, 68));
    liste.ajouterDescripteurs(Image("Lydia", "Grille01_gris.pgm", 69, 30, 'L', "gris", 4, 69));
    liste.ajouterDescripteurs(Image("Tom", "GrosArbre01_couleur.CR2", 70, 83, 'L', "couleur", 3, 70));
    liste.ajouterDescripteurs(Image("Tom", "GrosArbre01_gris.pgm", 71, 51, 'L', "gris", 3, 71));
    liste.ajouterDescripteurs(Image("Tom", "LaboPhonix_couleur.CR2", 72, 98, 'L', "couleur", 5, 72));
    liste.ajouterDescripteurs(Image("Tom", "LaboPhonix_gris.pgm", 73, 43, 'L', "gris", 4, 73));
    liste.ajouterDescripteurs(Image("Tom", "MagnifiquePontPret_couleur.CR2", 74, 72, 'L', "couleur", 4, 74));
    liste.ajouterDescripteurs(Image("Tom", "MagnifiquePontPret_gris.pgm", 75, 48, 'L', "gris", 4, 75));
    liste.ajouterDescripteurs(Image("Lydia", "Mario01_couleur.CR2", 76, 93, 'L', "couleur", 4, 76));
    liste.ajouterDescripteurs(Image("Lydia", "Mario01_gris.pgm", 77, 24, 'L', "gris", 3, 77));
    liste.ajouterDescripteurs(Image("Tom", "PlanteBeaucoup_couleur.CR2", 78, 92, 'L', "couleur", 4, 78));
    liste.ajouterDescripteurs(Image("Tom", "PlanteBeaucoup_gris.pgm", 79, 36, 'L', "gris", 4, 79));
    liste.ajouterDescripteurs(Image("Lydia", "PontPlante01_couleur.CR2", 80, 46, 'L', "couleur", 5, 80));
    liste.ajouterDescripteurs(Image("Lydia", "PontPlante01_gris.pgm", 81, 27, 'L', "gris", 4, 81));
    liste.ajouterDescripteurs(Image("Tom", "PontGens_couleur.CR2", 82, 58, 'L', "couleur", 4, 82));
    liste.ajouterDescripteurs(Image("Tom", "PontGens_gris.pgm", 83, 34, 'L', "gris", 4, 83));
    liste.ajouterDescripteurs(Image("Tom", "Rugby_couleur.CR2", 84, 72, 'L', "couleur", 5, 84));
    liste.ajouterDescripteurs(Image("Tom", "Rugby_gris.pgm", 85, 56, 'L', "gris", 4, 85));
    liste.ajouterDescripteurs(Image("Tom", "SousPontLumineux_couleur.CR2", 86, 89, 'L', "couleur", 4, 86));
    liste.ajouterDescripteurs(Image("Tom", "SousPontLumineux_gris.pgm", 87, 33, 'L', "gris", 4, 87));
    liste.ajouterDescripteurs(Image("Tom", "StatuLoin_couleur.CR2", 88, 82, 'L', "couleur", 5, 88));
    liste.ajouterDescripteurs(Image("Tom", "StatuLoin_gris.pgm", 89, 74, 'L', "gris", 4, 89));
    liste.ajouterDescripteurs(Image("Tom", "Ville_couleur.CR2", 90, 78, 'L', "couleur", 5, 90));
    liste.ajouterDescripteurs(Image("Tom", "Ville_gris.pgm", 91, 66, 'L', "gris", 4, 91));

    // >100€ :
    liste.ajouterDescripteurs(Image("Lydia", "BriqueSolLoin_couleur.CR2", 92, 110, 'L', "couleur", 4, 92));
    liste.ajouterDescripteurs(Image("Lydia", "BriqueSolLoin_gris.pgm", 93, 105, 'L', "gris", 4, 93));
    liste.ajouterDescripteurs(Image("Tom", "ChaussureLydia_couleur.CR2", 94, 10000, 'L', "couleur", 5, 94));
    liste.ajouterDescripteurs(Image("Tom", "ChaussureLydia_gris.pgm", 95, 5000, 'L', "gris", 4, 95));
    liste.ajouterDescripteurs(Image("Tom", "CotePont_couleur.CR2", 96, 153, 'L', "couleur", 4, 96));
    liste.ajouterDescripteurs(Image("Tom", "CotePont_gris.pgm", 97, 120, 'L', "gris", 4, 97));
    liste.ajouterDescripteurs(Image("Tom", "DessusPont_couleur.CR2", 98, 243, 'L', "couleur", 5, 98));
    liste.ajouterDescripteurs(Image("Tom", "DessusPont_gris.pgm", 99, 203, 'L', "gris", 4, 99));
    liste.ajouterDescripteurs(Image("Tom", "Dome_couleur.CR2", 100, 225, 'L', "couleur", 5, 100));
    liste.ajouterDescripteurs(Image("Tom", "Dome_gris.pgm", 101, 193, 'L', "gris", 4, 101));
    liste.ajouterDescripteurs(Image("Tom", "EauBord_couleur.CR2", 102, 546, 'L', "couleur", 4, 102));
    liste.ajouterDescripteurs(Image("Tom", "EauBord_gris.pgm", 103, 348, 'L', "gris", 3, 103));
    liste.ajouterDescripteurs(Image("Tom", "GruArbre_couleur.CR2", 104, 166, 'L', "couleur", 6, 104));
    liste.ajouterDescripteurs(Image("Tom", "GruArbre_gris.pgm", 105, 124, 'L', "gris", 5, 105));
    liste.ajouterDescripteurs(Image("Lydia", "MurArcEnCiel01_couleur.CR2", 106, 211, 'L', "couleur", 6, 106));
    liste.ajouterDescripteurs(Image("Lydia", "MurArcEnCiel01_gris.pgm", 107, 158, 'L', "gris", 5, 107));
    liste.ajouterDescripteurs(Image("Tom", "MurArcEnCiel02_couleur.CR2", 108, 213, 'L', "couleur", 6, 108));
    liste.ajouterDescripteurs(Image("Tom", "MurArcEnCiel02_gris.pgm", 109, 155, 'L', "gris", 5, 109));
    liste.ajouterDescripteurs(Image("Tom", "MurEauGens_couleur.CR2", 110, 321, 'L', "couleur", 5, 110));
    liste.ajouterDescripteurs(Image("Tom", "MurEauGens_gris.pgm", 111, 298, 'L', "gris", 4, 111));
    liste.ajouterDescripteurs(Image("Lydia", "PlantePont02_couleur.CR2", 112, 483, 'L', "couleur", 6, 112));
    liste.ajouterDescripteurs(Image("Lydia", "PlantePont02_gris.pgm", 113, 249, 'L', "gris", 5, 113));
    liste.ajouterDescripteurs(Image("Tom", "StatuPorte_couleur.CR2", 114, 647, 'L', "couleur", 6, 114));
    liste.ajouterDescripteurs(Image("Tom", "StatuPorte_gris.pgm", 115, 573, 'L', "gris", 5, 115));



    // Connexion

    // si connexion echouer alors recommencer une connexion
    while (user.getLogin() != "ad-01-ao" && user.getLogin() != "us-02-al") {
        user.connexion();
    }

    // boucle seulement pour si us-02-al le user est connecté
    if (user.getLogin() == "us-02-al") {
        std::cout << "Vous êtes connecter en tant que simple user (acces restreint)." << std::endl;
        liste.afficherGratuites();
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

        //liste.afficher(); A quoi ça sert de sauvegarder si tout les changements sont fais sur la liste ????
        liste.sauvegarderDansFichier("Biblio_init.txt");

        // modification de l'acces d'une image
        int numeroModification;
        std::cout << "Entrez le numéro de l'image à modifier l'accès : ";
        std::cin >> numeroModification;

        // Appel de la méthode pour modifier l'accès dans la liste
        liste.modifierAcces(numeroModification);
        // Afficher l'image numéro 30
        std::cout << liste.rechercherImageParNumero(30) << std::endl;
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
    liste.afficherImagesAvecAccesO("Biblio_init.txt");
    return 0;
}